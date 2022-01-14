#pragma once

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define ARGUMENT_MAX		3	// Max number of arguments

// Special instructions, 0-9 reserved
#define TYPE_NOP			0	// No operations
#define TYPE_HALT			1	// Halt program

// Branch instructions, 10-19 reserved, registers for arguments only
#define TYPE_BR				10	// Branch (no condition)
#define TYPE_BEQ			11	// Branch if REG1 == REG2
#define TYPE_BNE			12	// Branch if not equal
#define TYPE_BG				13	// Branch if ARG1 > ARG2
#define TYPE_BL				14	// Branch if ARG1 < ARG2
#define TYPE_BGE			15	// Branch if ARG1 >= ARG2
#define TYPE_BLE			16	// Branch if ARG1 <= ARG2

// Stack instructions, 20-29 reserved, registers for arguments only
#define TYPE_PUSH			20	// Push register onto stack
#define TYPE_POP			21	// Pop value on stack into register

// Arithmetic Operations, 100-199 reserved, registers only (for now)
#define TYPE_ADD			100	// Add Reg1 RegDest Reg2 (that is, let RegDest = Reg1 +  Reg2)
#define TYPE_SUB			101	// Same thing but subtract
#define TYPE_MUL			102	// Same thing but multiply
#define TYPE_DIV			103 // Same thing but divide
#define TYPE_MOD			104 // Same as division, but give the remainder as the result

// Register move operations, 200-299 reserved
#define TYPE_MOV_0			200	// Move immediate value into register 0. Use: MOV R0 [Immediate]
#define TYPE_MOV_1			201
#define TYPE_MOV_2			202
#define TYPE_MOV_3			203
#define TYPE_MOV_4			204
#define TYPE_MOV_5			205
#define TYPE_MOV_6			206
#define TYPE_MOV_7			207

// Register move operations, special, 300-399 reserved
#define TYPE_MOV_REG		300	// Move value in one register to another


struct Instruction {
	int type;
	int args[ARGUMENT_MAX];
	int numargs;
};

#endif // !INSTRUCTION_H


