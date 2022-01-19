#pragma once

#ifndef INSTRUCTIONDATA_H
#define INSTRUCTIONDATA_H

#include "instructionset.h"

extern struct InstructionWord systemop_words[];
extern struct InstructionWord branchop_words[];
extern struct InstructionWord stackop_words[];
extern struct InstructionWord aluop_words[];
extern struct InstructionWord movop_words[];

// Summary of all instructions
extern struct InstructionWordSection allwords[];
extern int numberofsections;

#endif // !INSTRUCTION_H


