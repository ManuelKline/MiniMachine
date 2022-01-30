#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "wordmatch.h"
#include "instructiondata.h"
#include "instructionset.h"
#include "memoryfile.h"

// Educational material on how functions-as-parameters work:
// https://math.hws.edu/bridgeman/courses/331/f05/handouts/c-c++-notes.html
// Might be useful here, when efficiency becomes more important

void instrn_halt() {
	exit(0);
}

// BRANCH INSTRUCTIONS:

void instrn_br(int relative_loc) {
	addpc(relative_loc);
}

void instrn_beq(int relative_loc, int reg1, int reg2) {
	if (getreg(reg1) == getreg(reg2)) {
		addpc(relative_loc);
	}
}

void instrn_bne(int relative_loc, int reg1, int reg2) {
	if (getreg(reg1) != getreg(reg2)) {
		addpc(relative_loc);
	}
}

void instrn_bg(int relative_loc, int reg1, int reg2) {
	if (getreg(reg1) > getreg(reg2)) {
		addpc(relative_loc);
	}
}

void instrn_bl(int relative_loc, int reg1, int reg2) {
	if (getreg(reg1) < getreg(reg2)) {
		addpc(relative_loc);
	}
}

void instrn_bge(int relative_loc, int reg1, int reg2) {
	if (getreg(reg1) >= getreg(reg2)) {
		addpc(relative_loc);
	}
}

void instrn_ble(int relative_loc, int reg1, int reg2) {
	if (getreg(reg1) <= getreg(reg2)) {
		addpc(relative_loc);
	}
}

// STACK INSTRUCTIONS:

void instrn_push(int reg) {
	push(getreg(reg));
}

void instrn_pop(int destreg) {
	int data = pop();

	setreg(destreg, data);
}

// ARITHMETIC OPERATIONS:

void instrn_add(int reg_dest, int reg_arg1, int reg_arg2) {
	int result = getreg(reg_arg1) + getreg(reg_arg2);

	setreg(reg_dest, result);
}

void instrn_sub(int reg_dest, int reg_arg1, int reg_arg2) {
	int result = getreg(reg_arg1) - getreg(reg_arg2);

	setreg(reg_dest, result);
}

void instrn_mul(int reg_dest, int reg_arg1, int reg_arg2) {
	int result = getreg(reg_arg1) * getreg(reg_arg2);

	setreg(reg_dest, result);
}

void instrn_div(int reg_dest, int reg_arg1, int reg_arg2) {
	int result = getreg(reg_arg1) / getreg(reg_arg2);

	setreg(reg_dest, result);
}

void instrn_mod(int reg_dest, int reg_arg1, int reg_arg2) {
	int result = getreg(reg_arg1) % getreg(reg_arg2);

	setreg(reg_dest, result);
}

// MOVE INSTRUCTIONS

void instrn_mov(int reg_dest, int immediate) {
	setreg(reg_dest, immediate);
}

// Rudimentary switching functions:
// I'll probably replace these with something more efficient
// Bitwise comparisons and hashmaps come to mind
// But for now, the goal is to make sure this is working.
void execute_system_type(struct Instruction* instrn) {
	switch (instrn->type)
	{
	case TYPE_HALT:
		instrn_halt();
		break;
	default:
		printf("Instruction not found in execution\n");
		exit(1);
		break;
	}
}

void execute_branch_type(struct Instruction* instrn) {
	switch (instrn->type)
	{
	case TYPE_BR:
		instrn_br(instrn->args[0]);
		break;
	case TYPE_BEQ:
		instrn_beq(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_BNE:
		instrn_bne(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_BG:
		instrn_bg(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_BL:
		instrn_bl(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_BGE:
		instrn_bge(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_BLE:
		instrn_ble(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	default:
		printf("Instruction not found in execution\n");
		exit(1);
		break;
	}
}

void execute_stack_type(struct Instruction* instrn) {
	switch (instrn->type)
	{
	case TYPE_PUSH:
		instrn_push(instrn->args[0]);
		break;
	case TYPE_POP:
		instrn_pop(instrn->args[0]);
		break;
	default:
		printf("Instruction not found in execution\n");
		exit(1);
		break;
	}
}

void execute_alu_type(struct Instruction* instrn) {
	switch (instrn->type)
	{
	case TYPE_ADD:
		instrn_add(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_SUB:
		instrn_sub(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_MUL:
		instrn_mul(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_DIV:
		instrn_div(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	case TYPE_MOD:
		instrn_mod(instrn->args[0], instrn->args[1], instrn->args[2]);
		break;
	default:
		printf("Instruction not found in execution\n");
		exit(1);
		break;
	}
}

void execute_move_type(struct Instruction* instrn) {
	switch (instrn->type)
	{
	case TYPE_MOV:
		instrn_mov(instrn->args[0], instrn->args[1]);
		break;
	default:
		printf("Instruction not found in execution\n");
		exit(1);
		break;
	}
}

int filter_type(struct Instruction* instrn) {
	if (instrn == NULL) {
		return 1;
	}

	int type = instrn->type;

	if (type >= SYSOP_START && type <= SYSOP_END) {
		execute_system_type(instrn);
	}
	else if (type >= BRCOP_START && type <= BRCOP_END) {
		execute_branch_type(instrn);
	}
	else if (type >= STKOP_START && type <= STKOP_END) {
		execute_stack_type(instrn);
	}
	else if (type >= ALUOP_START && type <= ALUOP_END) {
		execute_alu_type(instrn);
	}
	else if (type >= MOVOP_START && type <= MOVOP_END) {
		execute_move_type(instrn);
	}
	else {
		printf("Warning: instruction type fits no category!\n");
	}

	return 0;
}

void testmemory() {
	checkinitialized();
}

void testinstnload() {
	loadinstructionset_full();
}

void begin_execution() {
	while (1) {
		if (filter_type(getinstruction())) {
			break;
		}
	}
}
