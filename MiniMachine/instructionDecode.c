#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "instructionset.h"
#include "token.h"
#include "wordmatch.h"

#define TOKEN_BAD 0
#define TOKEN_NUM 1
#define TOKEN_SYM 2
#define TOKEN_REG 3

// Important ASCII Values:
#define ASCII_A_CAP 65
#define ASCII_Z_CAP 90
#define ASCII_A 97
#define ASCII_Z 122
#define ASCII_ZERO 48
#define ASCII_NINE 57
#define ASCII_DECAP_VAL 32

// Simple power function
int power(unsigned int base, unsigned int exp) {
	int result = 1;

	for (unsigned int i = 0; i < exp; i++) {
		result *= base;
	}
	
	return result;
}

// Simple char-to-integer function
int chartoint(char* asciiNum) {
	int result = 0;
	int buffer = 0;
	int digitNum = 0;
	
	if (asciiNum == NULL) {
		return 0;
	}

	// Starting from the least signficant digit
	for (int i = strlen(asciiNum) - 1; i >= 0; i--) {
		
		// Convert the digit to a valid integer
		buffer = asciiNum[i] - '0';

		// Add to result, multiply based on signficance
		result = result + (buffer * power(10, digitNum));

		// Prepare for next digit.
		digitNum++;
	}

	return result;
}

// Slice function for strings, returns a substring at indices [start]-[end]
char* slice(char* source, unsigned int start, unsigned int end) {
	char* dest = NULL;		// Destination string
	unsigned int destindex = 0;

	if (end >= start && source != NULL) {
		dest = (char*)calloc(1, end - start + 1);
		if (dest != NULL && strlen(source) >= end) {
			for (unsigned int i = start; i < end; i++) {
				dest[destindex] = source[i];
				destindex++;
			}
		}
		else {
			printf("Error in slice: dest calloc failed, or end index is too large\n");
			exit(1);
		}
	}
	else {
		printf("Error in slice: end < start, or source is null\n");
		exit(1);
	}

	return dest;
}

// Append function for strings, returns a string with appended character
char* append(char* dest, char newchar) {
	char* temp = NULL;		// Temp char* to hold new string
	int newLength = 0;

	// If dest is a valid string
	if (dest != NULL) {
		// Allocate space for new string
		newLength = strlen(dest) + 1;
		temp = (char*)calloc(1, newLength + 1);

		if (temp == NULL) {
			return NULL;
		}

		// Copy contents of old string into new string
		for (int i = 0; i < strlen(dest); i++) {
			temp[i] = dest[i];
		}

		// Append new character
		temp[newLength - 1] = newchar;

		return temp;
	}
	else {
		// Create a single character string and return it
		newLength = 1;
		temp = (char*)calloc(1, newLength + 1);

		if (temp == NULL) {
			return NULL;
		}

		temp[0] = newchar;

		return temp;
	}
}

struct Token* createtoken(char* literal, int literalSize, int type, int value) {
	struct Token* newToken = (struct Token*)calloc(1, sizeof(struct Token));

	// If calloc successful, set values
	if (newToken != NULL) {
		if (literal != NULL) {
			// Make room for the string literal and copy it
			newToken->literal = (char*)malloc(literalSize + 1);
			if (newToken->literal != NULL) {
				strcpy_s(newToken->literal, literalSize + 1, literal);
			}
			else {
				printf("Error in createtoken: failed to malloc literal\n");
				exit(1);
			}
		}
		newToken->type = type;
		newToken->value = value;
	}

	return newToken;
}

struct Instruction* createinstruction(int type, struct Argument arg1, struct Argument arg2, struct Argument arg3, int argnum) {
	struct Instruction* newInst = (struct Instruction*)calloc(1, sizeof(struct Instruction));

	// Set values if successful
	if (newInst != NULL) {
		newInst->type = type;
		newInst->args[0] = arg1;
		newInst->args[1] = arg2;
		newInst->args[2] = arg3;
		newInst->numargs = argnum;
	}

	return newInst;
}

struct Argument* create_arg(int type, int value1, int value2, int value3) {
	struct Argument* newArg = (struct Argument*)calloc(1, sizeof(struct Argument));

	if (newArg != NULL) {
		newArg->type = type;
		newArg->value_primary = value1;
		newArg->value_secondary = value2;
		newArg->value_tertiary = value3;
	}

	return newArg;
}

// tokenize: take a single line and convert it to tokens
struct Token* tokenize(char* input) {
	struct Token* currentToken = NULL;
	struct Token* prevToken = NULL;
	struct Token* firstToken = NULL;
	char* buffer = NULL;
	int bufferSize = 0;
	int bufferIsNum = 0;
	int bufferIsSym = 0;
	int bufferIsReg = 0;
	int lastSawR = 0;
	int negNum = 0;

	if (input == NULL) {
		return NULL;
	}

	for (unsigned int i = 0; i <= strlen(input); i++) {

		// If character is whitespace or EOF:
		if (input[i] == ' ' || input[i] == '\0') {
			// If buffer contains characters:
			if (bufferSize > 0) {
				// Create token based on type
				if (bufferIsNum && !bufferIsSym && !bufferIsReg) {
					if (negNum) {
						currentToken = createtoken(buffer, bufferSize, TOKEN_NUM, -1*chartoint(buffer));
					}
					else {
						currentToken = createtoken(buffer, bufferSize, TOKEN_NUM, chartoint(buffer));
					}
				}
				else if (!bufferIsNum && bufferIsSym && !bufferIsReg) {
					currentToken = createtoken(buffer, bufferSize, TOKEN_SYM, 0);
				}
				else if (!bufferIsNum && !bufferIsSym && bufferIsReg) {
					currentToken = createtoken(buffer, bufferSize, TOKEN_REG, chartoint(slice(buffer, 1, strlen(buffer))));
				}
				else {
					printf("Error: buffer is non-empty but is not clearly defined in type.\n");
					exit(1);
				}

				bufferIsNum = bufferIsSym = bufferIsReg = bufferSize = negNum = 0;
				free(buffer);
				buffer = NULL;

				// Record as first token if applicable
				if (firstToken == NULL) {
					firstToken = currentToken;
					prevToken = currentToken;
				}
				// If not, just update prevToken
				else {
					if (prevToken != NULL) {
						prevToken->nextToken = currentToken;
						prevToken = currentToken;
					}
				}
			}
		}
		// If character is alpha:
		else if ((ASCII_A <= input[i] && input[i] <= ASCII_Z) || (ASCII_A_CAP <= input[i] && input[i] <= ASCII_Z_CAP)) {
			// If numbers were read
			if (bufferIsNum && !bufferIsReg) {
				// Throw error, clear buffer, reset bools
				printf("Error: alpha read and buffer has numbers\n");
				bufferIsNum = bufferIsSym = bufferIsReg = bufferSize = 0;
				free(buffer);
				exit(1);
				break;
			}
			else if (!bufferIsNum && bufferIsReg) {
				// Throw error, clear buffer, reset bools
				printf("Error: alpha read and buffer comprises register\n");
				bufferIsNum = bufferIsSym = bufferIsReg = bufferSize = 0;
				free(buffer);
				exit(1);
				break;
			}
			else {
				// Add input to buffer
				bufferIsSym = 1;
				buffer = append(buffer, input[i]);
				bufferSize++;
				if (input[i] == 'R') {
					lastSawR = 1;
				}
			}
		}
		// If character is number
		else if (ASCII_ZERO <= input[i] && input[i] <= ASCII_NINE) {
			// If alphas were read
			if (bufferIsSym && !bufferIsReg) {
				// If last character was R, mark as possible register token
				if (lastSawR) {
					bufferIsSym = bufferIsNum = lastSawR = 0;
					bufferIsReg = 1;
					buffer = append(buffer, input[i]);
					bufferSize++;
				}
				else {
					// Throw error, clear buffer, reset bools
					printf("Error: number read and buffer has alphas\n");
					bufferIsNum = bufferIsSym = bufferIsReg = bufferSize = 0;
					free(buffer);
					exit(1);
					break;
				}
			}
			else if (!bufferIsSym && bufferIsReg) {
				// Add input to buffer
				buffer = append(buffer, input[i]);
				bufferSize++;
			}
			else {
				// Add input to buffer
				bufferIsNum = 1;
				buffer = append(buffer, input[i]);
				bufferSize++;
			}
		}
		// Else if character is a negative
		else if (input[i] == '-') {
			// If buffer has anything, this is invalid
			if (bufferIsSym || bufferIsReg || bufferIsNum) {
				printf("Error: negative symbol read, but characters preceeding it\n");
				free(buffer);
				exit(1);
			}
			else {
				negNum = 1;
				bufferIsNum = 1;
			}
		}
		// Else, character is invalid
		else {
			printf("Error: invalid character\n");
			exit(1);
			break;
		}
	}

	return firstToken;
}

// decode: take the tokens for a single instruction and return a single instruction struct
struct Instruction* instructionize(struct Token* firstToken) {
	struct Token* currentToken = firstToken;
	struct Instruction* inst = (struct Instruction*)calloc(1, sizeof(struct Instruction));
	int sym_counter = 0;
	int numargs = 0;
	int debug_counter = 0;

	if (inst == NULL) {
		printf("Error in decode: calloc instruction failed\n");
		return NULL;
	}

	while (currentToken != NULL && debug_counter < ARGUMENT_MAX + 1) {
		switch (currentToken->type)
		{
		case TOKEN_SYM:
			if (sym_counter == 0) {
				sym_counter++;
				// Use wordmatch to find specific command
				inst->type = find_word(currentToken->literal);
				if (inst->type == -1) {
					printf("Warning: unknown instruction: %s\n", currentToken->literal);
				}
			}
			else {
				printf("Error in decode: two symbol tokens present\n");
				exit(1);
			}
			break;
		case TOKEN_NUM:
			if (sym_counter) {
				if (numargs < ARGUMENT_MAX) {
					// Insert type and numerical value at args[numargs]
					inst->args[numargs].type = ARG_IMM;
					inst->args[numargs].value_primary = currentToken->value;
					numargs++;
				}
				else {
					printf("Error in decode: Exceeding max number of args\n");
					exit(1);
				}
			}
			else {
				printf("Error in decode: first token is register, not a command\n");
				exit(1);
			}
			break;
		case TOKEN_REG:
			if (sym_counter) {
				if (numargs < ARGUMENT_MAX) {
					// Inserttype and numerical value at args[numargs]
					inst->args[numargs].type = ARG_REG;
					inst->args[numargs].value_primary = currentToken->value;
					numargs++;
				}
				else {
					printf("Error in decode: Exceeding max number of args\n");
					exit(1);
				}
			}
			else {
				printf("Error in decode: first token is register, not a command\n");
				exit(1);
			}
			break;
		default:
			printf("Error in decode: instruction type undefined\n");
			exit(1);
			break;
		}

		currentToken = currentToken->nextToken;
		debug_counter++;
	}

	inst->numargs = numargs;

	return inst;
}

struct Instruction* decodeline(char* input) {
	return instructionize(tokenize(input));
}