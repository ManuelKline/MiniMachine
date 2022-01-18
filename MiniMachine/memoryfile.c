#include "stdio.h"
#include "stdlib.h"
#include "instruction.h"

#define INSTN_BLOCKSIZE_DEFAULT 8;				// Recommend 2048
#define STACK_SIZE_DEFAULT 2048;

// ***** Stack *****
static int* stack = NULL;
static int mem_stacksize = STACK_SIZE_DEFAULT;	
static int sp = -1;								// Stack pointer

// ***** Register File *****
static int* regfile = NULL;
static int mem_regfilesize = 0;

// ***** Instruction Memory (IM) *****
struct Instn_Block_Listing {					// Struct type for IM
	struct Instruction** instn_block;			// Array of pointers to instructions
	struct Instn_Block_Listing* next_block;		// Pointer to next block
};
static struct Instn_Block_Listing* first_block = NULL;
static unsigned int instn_blocksize = INSTN_BLOCKSIZE_DEFAULT;
static int pc = 0;

// !*****! DEPRECATED !*****!:
static int mem_instmemsize = 0;
static struct Instruction** instMemory = NULL;	// Array of pointers to instructions

// Initialization Boolean
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
	if (location >= 0) {
		pc = location;
	}
}

// Instruction branch relative position
void addpc(int branch) {
	int newlocation = pc + branch;
	setpc(newlocation);
}

// Instruction input new instruction
void addinstruction(struct Instruction* data) {
	struct Instn_Block_Listing* block_location = first_block;
	pc++;

	// Goto block listing that pc resides
	for (int i = 0; i < pc / instn_blocksize; i++) {
		if (block_location != NULL) {
			// Goto next if possible
			block_location = block_location->next_block;
		}
		else {
			// Allocate new block
			block_location = (struct Instn_Block_Listing*)calloc(1, sizeof(struct Instn_Block_Listing));
			if (block_location != NULL) {
				block_location = block_location->next_block;
			}
			else {
				printf("Error in addinstruction: block listing allocation failed\n");
			}
		}
	}

	// Add instruction to location given by pc:
	if (block_location != NULL) {
		if (block_location->instn_block != NULL) {
			block_location->instn_block[pc % instn_blocksize] = data;
		}
		else {
			block_location->instn_block = (struct Instruction**)calloc(1, sizeof(struct Instruction*) * instn_blocksize);
			if (block_location->instn_block != NULL) {
				block_location->instn_block[pc % instn_blocksize] = data;
			}
			else {
				printf("Error in addinstruction: block allocation failed\n");
			}
		}
	}
	else {
		block_location = (struct Instn_Block_Listing*)calloc(1, sizeof(struct Instn_Block_Listing));
		if (block_location != NULL) {
			block_location->instn_block = (struct Instruction**)calloc(1, sizeof(struct Instruction*) * instn_blocksize);
			if (block_location->instn_block != NULL) {
				block_location->instn_block[pc % instn_blocksize] = data;
			}
			else {
				printf("Error in addinstruction: block allocation failed\n");
			}
		}
		else {
			printf("Error in addinstruction: block listing allocation failed\n");
		}
	}
}

// Instruction get instruction
struct Instruction* getinstruction() {
	struct Instruction* data = NULL;
	struct Instn_Block_Listing* block_location = first_block;

	// Goto block listing that pc resides
	for (int i = 0; i < pc / instn_blocksize; i++) {
		if (block_location != NULL) {
			// Goto next if possible
			block_location = block_location->next_block;
		}
		else {
			printf("Error in getinstruction: block listing does not exist\n");
		}
	}

	// Get instruction at location given by pc:
	if (block_location != NULL) {
		if (block_location->instn_block != NULL) {
			data = block_location->instn_block[pc % instn_blocksize];
		}
		else {
			printf("Error in getinstruction: block does not exist\n");
		}
	}
	else {
		printf("Error in getinstruction: block listing does not exist\n");
	}

	pc++;
	return data;
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
void initialize(unsigned int stacksize, unsigned int regfilesize) {
	mem_stacksize = stacksize;
	mem_regfilesize = regfilesize;
	//mem_instmemsize = instmemsize;

	// Allocate to stack and register files as arrays of ints
	stack = (int*)malloc(sizeof(int) * stacksize);
	regfile = (int*)malloc(sizeof(int) * regfilesize);
	//instMemory = (struct Instruction**)malloc(sizeof(struct Instruction*) * instmemsize);

	// Allocate first listing of block list
	first_block = (struct Instn_Block_Listing*)calloc(1, sizeof(struct Instn_Block_Listing));
	if (first_block == NULL) {
		printf("Error in initialize: first_block failed malloc\n");
	}
	else {
		// Allocate first block (array of pointers to instructions)
		first_block->instn_block = (struct Instruction**)calloc(1, sizeof(struct Instruction*) * instn_blocksize);
	}

	// Set memory pointers and boolean
	sp = -1;
	pc = 0;
	initialized = 1;
}

void free_instn_block(struct Instruction** current_block) {
	if (current_block != NULL) {
		free(current_block);
	}
}

void free_instn_listing(struct Instn_Block_Listing* current_listing) {
	if (current_listing->next_block != NULL) {
		free_instn_listing(current_listing->next_block);
	}

	free_instn_block(current_listing->instn_block);
	free(current_listing);
}

void free_instn_memory() {
	free_instn_listing(first_block);
}

// Free static variables, set everything to empty
void destroymem() {
	free(stack);
	free(regfile);
	//free(instMemory);
	free_instn_memory();
	sp = -1;
	pc = 0;
	mem_regfilesize = mem_stacksize = 0;

	initialized = 0;
}
