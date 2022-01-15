#pragma once

#ifndef MEMORYFILE_H
#define MEMORYFILE_H

void push(int data);
int pop();
void step();
void setpc(int location);
void addpc(int branch);
void addinstruction(struct Instruction* data);
int getreg(int regnum);
void setreg(int regnum, int data);
void checkinitialized();
void initialize(unsigned int stacksize, unsigned int regfilesize, unsigned int instmemsize);
void destroymem();

#endif // !MEMORYFILE_H
