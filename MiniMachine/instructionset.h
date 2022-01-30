#pragma once
#ifndef INSTRUCTIONSET_H
#define INSTRUCTIONSET_H

// Constants:
#define ARGUMENT_MAX		3	// Max number of arguments

// Special (system) instructions, 0-9 reserved
#define TYPE_NOP			0	// No operations
#define TYPE_HALT			1	// Halt program

#define SYSOP_START			0
#define SYSOP_END			9
#define SYSOP_SIZE			2

// Branch instructions, 10-19 reserved, registers for arguments only
#define TYPE_BR				10	// Branch (no condition)
#define TYPE_BEQ			11	// Branch if REG1 == REG2
#define TYPE_BNE			12	// Branch if not equal
#define TYPE_BG				13	// Branch if ARG1 > ARG2
#define TYPE_BL				14	// Branch if ARG1 < ARG2
#define TYPE_BGE			15	// Branch if ARG1 >= ARG2
#define TYPE_BLE			16	// Branch if ARG1 <= ARG2
// Usage example: BLE 5 R0 R1

#define BRCOP_START			10
#define BRCOP_END			19
#define BRCOP_SIZE			7

// Stack instructions, 20-29 reserved, registers for arguments only
#define TYPE_PUSH			20	// Push register onto stack
#define TYPE_POP			21	// Pop value on stack into register

#define STKOP_START			20
#define STKOP_END			29
#define STKOP_SIZE			2

// Arithmetic Operations, 100-199 reserved, registers only (for now)
#define TYPE_ADD			100	// Add RegDest Reg1 Reg2 (that is, let RegDest = Reg1 +  Reg2)
#define TYPE_SUB			101	// Same thing but subtract
#define TYPE_MUL			102	// Same thing but multiply
#define TYPE_DIV			103 // Same thing but divide
#define TYPE_MOD			104 // Same as division, but give the remainder as the result

#define ALUOP_START			100
#define ALUOP_END			199
#define ALUOP_SIZE			5

// Register move operations, 200-210 reserved
#define TYPE_MOV			200	// Move immediate value into register 0. Use: MOV R0 [Immediate]
//#define TYPE_MOV_REG		201	// Move value in one register to another

#define MOVOP_START			200
#define MOVOP_END			210
#define MOVOP_SIZE			1

// Argument section:
#define ARG_NUL 0	// Null argument - doesn't exist.
#define ARG_REG 1	// Register argument - a register number to be fetched
#define ARG_IMM 2	// Immediate argument - an immediate number
#define ARG_ADR	3	// Address argument (planned to be used for memory ops)
#define ARG_OFS 4	// Offset argument (planned to be used for memory ops)

// Definitions:

struct Argument {
	int type;
	int value_primary;		// Primary value - often the only one that matters.
	int value_secondary;	// Secondary value - when a number becomes two-dimensional.
	int value_tertiary;		// Tertiary value - don't know what I'd use this for, but it's there.
};

struct Instruction {
	int type;
	struct Argument args[ARGUMENT_MAX];
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

#endif // !INSTRUCTIONSET_H
