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

// Argument extraction function
int arg_extract(struct Argument* arg) {
	switch (arg->type)
	{
	case ARG_REG:
		return getreg(arg->value_primary);
		break;
	case ARG_IMM:
		return arg->value_primary;
		break;
	default:
		printf("Warning in arg_extract_value: argument %d type not supported\n", arg->type);
		return 0;
		break;
	}
}

void instrn_halt() {
	exit(0);
}

// BRANCH INSTRUCTIONS:
void instrn_br(struct Argument arg1) {
	addpc(arg_extract(&arg1));
}

void instrn_beq(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	if (arg_extract(&arg2) == arg_extract(&arg3)) {
		addpc(arg_extract(&arg1));
	}
}

void instrn_bne(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	if (arg_extract(&arg2) != arg_extract(&arg3)) {
		addpc(arg_extract(&arg1));
	}
}

void instrn_bg(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	if (arg_extract(&arg2) > arg_extract(&arg3)) {
		addpc(arg_extract(&arg1));
	}
}

void instrn_bl(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	if (arg_extract(&arg2) < arg_extract(&arg3)) {
		addpc(arg_extract(&arg1));
	}
}

void instrn_bge(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	if (arg_extract(&arg2) >= arg_extract(&arg3)) {
		addpc(arg_extract(&arg1));
	}
}

void instrn_ble(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	if (arg_extract(&arg2) <= arg_extract(&arg3)) {
		addpc(arg_extract(&arg1));
	}
}

// STACK INSTRUCTIONS:

// Can push either immediate or register value
void instrn_push(struct Argument arg1) {
	push(arg_extract(&arg1));
}

// Can pop into register, must be valid argument
void instrn_pop(struct Argument arg1) {
	// Check argument
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_pop: destination is not a register\n");
		exit(1);
	}

	int data = pop();

	setreg(arg1.value_primary, data);
}

// ARITHMETIC OPERATIONS:

void instrn_add(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	// Check that destination is valid
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_add: destination is not a register\n");
		exit(1);
	}

	int result = arg_extract(&arg2) + arg_extract(&arg3);

	setreg(arg1.value_primary, result);
}

void instrn_sub(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	// Check that destination is valid
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_syb: destination is not a register\n");
		exit(1);
	}

	int result = arg_extract(&arg2) - arg_extract(&arg3);

	setreg(arg1.value_primary, result);
}

void instrn_mul(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	// Check that destination is valid
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_mul: destination is not a register\n");
		exit(1);
	}

	int result = arg_extract(&arg2) * arg_extract(&arg3);

	setreg(arg1.value_primary, result);
}

void instrn_div(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	// Check that destination is valid
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_div: destination is not a register\n");
		exit(1);
	}

	int result = arg_extract(&arg2) / arg_extract(&arg3);

	setreg(arg1.value_primary, result);
}

void instrn_mod(struct Argument arg1, struct Argument arg2, struct Argument arg3) {
	// Check that destination is valid
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_mod: destination is not a register\n");
		exit(1);
	}

	int result = arg_extract(&arg2) % arg_extract(&arg3);

	setreg(arg1.value_primary, result);
}

// MOVE INSTRUCTIONS

// Destination (arg1) must be register
// Source may be register or immediate
void instrn_mov(struct Argument arg1, struct Argument arg2) {
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_mov: destination is not a register\n");
		exit(1);
	}

	if (arg2.type != ARG_REG && arg2.type != ARG_IMM) {
		printf("Error in instrn_mov: source is not a register nor immediate\n");
		exit(1);
	}

	setreg(arg1.value_primary, arg_extract(&arg2));
}

// MEMORY INSTRUCTIONS

// Destination (arg1) must be register
// Source must be immediate (address)
// Yes, very simplified version of LDR
void instrn_ldr(struct Argument arg1, struct Argument arg2) {
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_ldr: destination is not a register\n");
		exit(1);
	}

	if (arg2.type != ARG_IMM) {
		printf("Error in instrn_ldr: address is not an immediate\n");
		exit(1);
	}

	setreg(arg1.value_primary, load((unsigned int)arg_extract(&arg2)));
}

// Source (arg1) must be register
// Destination must be immediate (address)
// Yes, very simplified version of STR
void instrn_str(struct Argument arg1, struct Argument arg2) {
	if (arg1.type != ARG_REG) {
		printf("Error in instrn_str: destination is not a register\n");
		exit(1);
	}

	if (arg2.type != ARG_IMM) {
		printf("Error in instrn_str: address is not an immediate\n");
		exit(1);
	}

	store((unsigned int)arg_extract(&arg1), ((unsigned int)arg_extract(&arg2)));
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

void execute_mem_type(struct Instruction* instrn) {

	switch (instrn->type)
	{
	case TYPE_LDR:
		instrn_ldr(instrn->args[0], instrn->args[1]);
		break;
	case TYPE_STR:
		instrn_str(instrn->args[0], instrn->args[1]);
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
	else if (type >= MEMOP_START && type <= MEMOP_END) {
		execute_mem_type(instrn);
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
