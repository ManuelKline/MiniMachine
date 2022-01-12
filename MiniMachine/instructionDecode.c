#include "stdio.h"
#include "instruction.h"
#include "token.h"

#define TOKEN_BAD 0
#define TOKEN_NUM 1
#define TOKEN_SYM 2

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

	//if (exp == 0) {
	//	result = 1;
	//}

	for (unsigned int i = 0; i < exp; i++) {
		result *= base;
	}

	//printf("DIGIT SIGNIFICANCE MULTIPLICATION RESULT: %d\n", result);
	return result;
}

// Simple char-to-integer function
int chartoint(char* asciiNum) {
	int result = 0;
	int buffer = 0;
	int digitNum = 0;

	printf("chartoint called with: %s\n", asciiNum);

	if (asciiNum == NULL) {
		return 0;
	}

	// Starting from the least signficant digit
	for (int i = strlen(asciiNum) - 1; i >= 0; i--) {
		//printf("FOR-LOOP INDEX: %d\n", i);
		//printf("DIGIT in CHAR: %c\n", asciiNum[i]);

		// Convert the digit to a valid integer
		buffer = asciiNum[i] - '0';

		//printf("DIGIT in INT: %d\n", buffer);

		// Add to result, multiply based on signficance
		result = result + (buffer * power(10, digitNum));

		// Prepare for next digit.
		digitNum++;
	}

	return result;
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
			//printf("Loop 1 index %d, character %c\n", i, dest[i]);
			temp[i] = dest[i];
		}

		//printf("Current temp: %s\n", temp);
		// Append new character
		temp[newLength - 1] = newchar;

		//printf("Current temp: %s\n", temp);

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
		//printf("Current temp: %s\n", temp);

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
				// Throw error
			}
		}
		newToken->type = type;
		newToken->value = value;
	}

	return newToken;
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

	if (input == NULL) {
		return NULL;
	}

	for (unsigned int i = 0; i < strlen(input); i++) {

		// If character is whitespace:
		if (input[i] == ' ') {
			// If buffer contains characters:
			if (bufferSize > 0) {
				// Create token based on type
				if (bufferIsNum && !bufferIsSym) {
					currentToken = createtoken(buffer, bufferSize, TOKEN_NUM, chartoint(buffer));
				}
				else if (bufferIsSym && !bufferIsNum) {
					currentToken = createtoken(buffer, bufferSize, TOKEN_SYM, 0);
				}
				else {
					printf("Error: buffer is non-empty but is neither number nor symbol\n");
				}

				bufferIsNum = bufferIsSym = 0;

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
			if (bufferIsNum) {
				// Throw error, clear buffer, reset bools
				bufferIsNum = bufferIsSym = bufferSize = 0;
				free(buffer);
				break;
			}
			else {
				// Add input to buffer
				bufferIsSym = 1;
				buffer = append(buffer, input[i]);
				bufferSize++;
			}
		}
		// If character is number
		else if (ASCII_ZERO <= input[i] && input[i] <= ASCII_NINE) {
			// If alphas were read
			if (bufferIsSym) {
				// Throw error, clear buffer, reset bools
				bufferIsNum = bufferIsSym = bufferSize = 0;
				free(buffer);
				break;
			}
			else {
				// Add input to buffer
				bufferIsNum = 1;
				buffer = append(buffer, input[i]);
				bufferSize++;
			}
		}
		// Else, character is invalid
		else {
			printf("Error: invalid character\n");
			break;
		}
	}

	return firstToken;
}

// decode: take the tokens for a single instruction and return a single instruction struct
struct Instruction* decode(struct Token* firstToken) {

}