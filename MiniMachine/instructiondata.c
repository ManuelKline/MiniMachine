#include "instructionset.h"


struct Instruction {
	int type;
	int args[ARGUMENT_MAX];
	int numargs;
};

struct InstructionWord {
	int type;
	char* literal;
};

struct InstructionWordSection {
	int start;
	int stop;
	int size;
	struct InstructionWord* words;
};

// Data section

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
int numberofsections = 5;