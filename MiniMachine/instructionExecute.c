#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "wordmatch.h"
#include "instructiondata.h"
#include "instructionset.h"
#include "memoryfile.h"

// Handy macro I found: https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
#define ARRAYSIZE(x)	(sizeof(x) / sizeof((x)[0]))

void testmemory() {
	checkinitialized();
}


void loadinstructionset_section(struct InstructionWord* words, int size) {
	for (int i = 0; i < size; i++) {
		printf("Adding word: %s, Type: %d\n", words[i].literal, words[i].type);
		add_word(words[i].literal, words[i].type);
	}
}


void loadinstructionset_full() {
	for (int i = 0; i < numberofsections; i++) {
		loadinstructionset_section(allwords[i].words, allwords[i].size);
	}
}
