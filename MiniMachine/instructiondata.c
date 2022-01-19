#include "instructionset.h"
#include "wordmatch.h"

// Definitions:

struct InstructionWord systemop_words[] = {
	{TYPE_NOP, "NOP"},
	{TYPE_HALT, "HALT"}
};

struct InstructionWord branchop_words[] = {
	{TYPE_BR, "BR"},
	{TYPE_BEQ, "BEQ"},
	{TYPE_BNE, "BNE"},
	{TYPE_BG, "BG"},
	{TYPE_BL, "BL"},
	{TYPE_BGE, "BGE"},
	{TYPE_BLE, "BLE"}
};

struct InstructionWord stackop_words[] = {
	{TYPE_PUSH, "PUSH"},
	{TYPE_POP, "POP"}
};

struct InstructionWord aluop_words[] = {
	{TYPE_ADD, "ADD"},
	{TYPE_SUB, "SUB"},
	{TYPE_MUL, "MUL"},
	{TYPE_DIV, "DIV"},
	{TYPE_MOD, "MOD"}
};

struct InstructionWord movop_words[] = {
	{TYPE_MOV, "MOV"}
};

// Summary of all instructions

struct InstructionWordSection allwords[] = {
	{SYSOP_START, SYSOP_END, SYSOP_SIZE, systemop_words},
	{BRCOP_START, BRCOP_END, BRCOP_SIZE, branchop_words},
	{STKOP_START, STKOP_END, STKOP_SIZE, stackop_words},
	{ALUOP_START, ALUOP_END, ALUOP_SIZE, aluop_words},
	{MOVOP_START, MOVOP_END, MOVOP_SIZE, movop_words}
};
const int numberofsections = 5;

// Handy macro I found: https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
#define ARRAYSIZE(x)	(sizeof(x) / sizeof((x)[0]))


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