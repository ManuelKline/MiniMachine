#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"

static FILE* fp;

// Append function for strings, returns a string with appended character
// Should probably add this to a separate file, since it is becoming useful
char* append_if(char* dest, char newchar) {
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
		for (unsigned int i = 0; i < strlen(dest); i++) {
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

// Getline, my own version, no length needed.
int getline(char** lineptr, FILE* stream) {
	int length = 0;
	char data = EOF;

	if (lineptr == NULL || stream == NULL) {
		printf("Error in getline: invalid argument\n");
		return -1;
	}

	printf("getline: arguments valid\n");

	// If necessary, allocate memory for single character
	if (*lineptr == NULL) {
		printf("getline: lineptr null, calloc\n");
		// Don't allocate space, but allow for reallocation
		*lineptr = (char*)calloc(1, 1);
		printf("getline: calloc done\n");
	}

	printf("getline: lineptr ready\n");

	data = fgetc(stream);
	printf("getline: fgetc done\n");
	while (data != EOF && data != '\n') {
		printf("getline lineptr: %s\n", *lineptr);
		printf("getline: calling append with data: %d\n", data);
		*lineptr = append_if(*lineptr, data);
		printf("getline: append done\n");
		if (*lineptr == NULL) {
			printf("Error in getline: append returned null pointer\n");
			return -1;
		}
		length++;
		data = fgetc(stream);
	}

	return length;
}

/*
char* fetchinstruction(const char* filename) {
	char* line = NULL;
	int length;
	errno_t err;

	printf("File open calling\n");
	err = fopen_s(&fp, filename, "r");
	printf("File open called: %d\n", err);

	if (fp != NULL) {
		// Read single line of any size
		length = getline(&line, fp);
		if (length == -1) {
			printf("Error in fetchinstruction: getline returned -1\n");
			return NULL;
		}
	}
	else {
		printf("Error in fetchinstruction: file could not be opened\n");
		return NULL;
	}

	if (line != NULL) {
		return line;
	}
	return NULL;
}
*/

char* fetchinstruction() {
	char* line = NULL;
	int length;
	errno_t err;

	if (fp != NULL) {
		// Read single line of any size
		length = getline(&line, fp);
		if (length == -1) {
			printf("Error in fetchinstruction: getline returned -1\n");
			return NULL;
		}
	}
	else {
		printf("Error in fetchinstruction: file could not be opened\n");
		return NULL;
	}

	return line;
}

int open_file_read(const char* filename) {
	errno_t err = fopen_s(&fp, filename, "r");
	return err;
}

int close_file() {
	int err = fclose(fp);
	return err;
}