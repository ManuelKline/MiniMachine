#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"

static FILE* fp;

// Append function for strings, returns a string with appended character
// Should probably add this to a separate file, since it is becoming useful
char* append_f(char* dest, char newchar) {
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

// Getline, my own version, no length needed.
int getline(char** lineptr, FILE* stream) {
	int length = 0;
	char data = EOF;

	if (lineptr == NULL || stream == NULL) {
		printf("Error in getline: invalid argument\n");
		return -1;
	}

	// If necessary, allocate memory for single character
	if (*lineptr == NULL) {
		// Don't allocate space, but allow for reallocation
		*lineptr = (char*)calloc(1, 1);
	}

	// Get the first character
	data = fgetc(stream);
	// If first character indicates EOF, return -1
	if (data == EOF) {
		return -1;
	}

	// This loop generates the full line
	while (data != EOF && data != '\n') {
		*lineptr = append_f(*lineptr, data);
		if (*lineptr == NULL) {
			printf("Error in getline: append returned null pointer\n");
			return -1;
		}
		length++;
		data = fgetc(stream);
	}

	return length;
}

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