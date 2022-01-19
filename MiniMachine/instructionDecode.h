#pragma once

#ifndef INSTRUCTIONDECODE_H
#define INSTRUCTIONDECODE_H

struct Token* tokenize(char* input);
struct Instruction* instructionize(struct Token* firstToken);
struct Instruction* decodeline(char* input);
char* slice(char* source, unsigned int start, unsigned int end);

#endif

