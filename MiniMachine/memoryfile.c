#include "stdio.h"
#include "stdlib.h"
#include "instruction.h"

static int* stack;
static int* regfile;
static struct Instruction** instMemory;	// Array of pointers to instructions
static int sp = -1;
static int pc = 0;
static int mem_stacksize = 0;
static int mem_regfilesize = 0;
static int mem_instmemsize = 0;

static int initialized = 0;

// Stack push
void push(int data) {
	sp++;
	if (sp < mem_stacksize) {
		stack[sp] = data;
	}
	else {
		// Error
	}
}

// Stack pop
int pop() {
	int data = 0;
	
	if (sp > -1) {
		data = stack[sp];
		sp--;
	}
	else {
		// Error
	}

	return data;
}

// Instruction step++
void step() {
	pc++;
}

// Instruction set program counter
void setpc(int location) {
	if (location >= 0 && location < instMemory) {
		pc = location;
	}
	else {
		// Error
	}
}

// Instruction branch relative position
void addpc(int branch) {
	int newlocation = pc + branch;
	setpc(newlocation);
}

// Instruction input new instruction
void addinstruction(struct Instruction* data) {
	pc++;
	if (pc < mem_instmemsize) {
		if (instMemory != NULL) {
			instMemory[pc] = data;
		}
	}
}

// Register get data
int getreg(int regnum) {
	int data = 0;

	if (regnum >= 0 && regnum < mem_regfilesize) {
		data = regfile[regnum];
	}
	else {
		// Error
	}

	return data;
}

// Register set data
void setreg(int regnum, int data) {
	if (regnum >= 0 && regnum < mem_regfilesize) {
		regfile[regnum] = data;
	}
	else {
		// Error
	}
}

// Basic test function
void checkinitialized() {
	if (initialized) {
		printf("Initialized!\n");
	}
	else {
		printf("Not initialized.\n");
	}
}

// Initialize static variables
void initialize(unsigned int stacksize, unsigned int regfilesize, unsigned int instmemsize) {
	mem_stacksize = stacksize;
	mem_regfilesize = regfilesize;
	mem_instmemsize = instmemsize;

	stack = (int*)malloc(sizeof(int) * stacksize);
	regfile = (int*)malloc(sizeof(int) * regfilesize);
	instMemory = (struct Instruction**)malloc(sizeof(struct Instruction*) * instmemsize);
	sp = -1;
	pc = 0;

	initialized = 1;
}

// Free static variables, set everything to empty
void destroymem() {
	free(stack);
	free(regfile);
	free(instMemory);
	sp = -1;
	pc = 0;
	mem_instmemsize = mem_regfilesize = mem_stacksize = 0;

	initialized = 0;
}
