#pragma once

#ifndef INSTRUCTIONDECODE_H
#define INSTRUCTIONDECODE_H

struct Token* tokenize(char* input);
struct Instruction* decode(struct Token* firstToken);

char* slice(char* source, unsigned int start, unsigned int end);

#endif

