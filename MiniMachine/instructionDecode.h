#pragma once

#ifndef INSTRUCTIONDECODE_H
#define INSTRUCTIONDECODE_H

struct Token* tokenize(char* input);
struct Instruction* decode(struct Token* firstToken);

#endif

