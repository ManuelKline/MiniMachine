#pragma once

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "instructionset.h"

// Schema section

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

extern struct InstructionWord systemop_words[];
extern struct InstructionWord branchop_words[];
extern struct InstructionWord stackop_words[];
extern struct InstructionWord aluop_words[];
extern struct InstructionWord movop_words[];

// Summary of all instructions
extern struct InstructionWordSection allwords[];
extern int numberofsections;

#endif // !INSTRUCTION_H


