// MiniMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "token.h"
#include "instructionDecode.h"
#include "instructionExecute.h"
#include "instructionFetch.h"
#include "instruction.h"
#include "memoryfile.h"
#include "wordmatch.h"

int main()
{
    char* bigstring = "ABCDEF";
    char* substring = NULL;
    struct Token* mytoken = NULL;
    struct Token* current = NULL;
    struct Instruction* myinst = NULL;
    int loopIndex = 0;

    // Tokenize testing
    mytoken = tokenize("ADD R13 R12 R11");
    current = mytoken;

    while (1) {
        if (current != NULL) {
            printf("Token %d:\n", loopIndex);
            printf("  Literal: %s\n", current->literal);
            printf("  Type:    %d\n", current->type);
            printf("  Value:   %d\n", current->value);
            printf("\n");
        }
        else {
            printf("End of token sequence.\n");
            break;
        }
        current = current->nextToken;
        loopIndex++;
    }

    free(mytoken);
    mytoken = NULL;

    mytoken = tokenize("ROR R13 R12");
    current = mytoken;

    while (1) {
        if (current != NULL) {
            printf("Token %d:\n", loopIndex);
            printf("  Literal: %s\n", current->literal);
            printf("  Type:    %d\n", current->type);
            printf("  Value:   %d\n", current->value);
            printf("\n");
        }
        else {
            printf("End of token sequence.\n");
            break;
        }
        current = current->nextToken;
        loopIndex++;
    }

    free(mytoken);
    mytoken = NULL;

    // Decode testing
    add_word("ADD", TYPE_ADD);
    myinst = decodeline("ADD R13 R12 R11");

    if (myinst != NULL) {
        printf("Instruction Type: %d\n", myinst->type);
        printf("Number of arguments: %d\n", myinst->numargs);

        for (int i = 0; i < myinst->numargs; i++) {
            printf("Argument %d: %d\n", i, myinst->args[i]);
        }
    }

    // Slice test
    substring = slice(bigstring, 2, strlen(bigstring));
    printf("Substring: %s\n", substring);

    // Memory file test
    initialize(16, 8);
    testmemory();
    destroymem();
    testmemory();
    printf("Mem test concluded\n");

    // Fetch test
    open_file_read("example.txt");
    printf("Instruction: (%s)\n", fetchinstruction());
    printf("Instruction: (%s)\n", fetchinstruction());
    printf("Instruction: (%s)\n", fetchinstruction());
    printf("Instruction: (%s)\n", fetchinstruction());
    close_file();

    // Fetch test 2
    open_file_read("example.txt");
    printf("Instruction: (%s)\n", fetchinstruction());
    printf("Instruction: (%s)\n", fetchinstruction());
    printf("Instruction: (%s)\n", fetchinstruction());
    printf("Instruction: (%s)\n", fetchinstruction());
    close_file();
}


/*
DESIGN CONSIDERATIONS:
    Design Issue 1: Scope of stack, SP, PC, register file, instruction memory:
        Solution 1:
            Define these in main(). Define struct holding pointers to all globally availiable points of data.
            Pass this struct to relevant functions (ie. instruction execution functions).
        Solution 2:
            Define these in a separate file. Allow others files to access static variables through interface.
    Design Issue 2: Software Architecture:
        Solution 1: Pipe and Filter
            Call sequence that starts at main(), ends at execute(). Communication resticted one-way. Conflicts
            with Solution 1 of Design Issue 1 (communication must exist between main() and execute() to pass
            stack, SP, PC, register file).
        Solution 2: Pipe and Filter + Event-Based Data (Hybrid Style)
            Call sequence that follows previous solution, but data (stack, SP, etc.) is defined in separate
            file that is included by other files that need it. When data is updated, function regarding
            execution of instructions receives signal (in this case, a simple call is sufficient).
            Removes need for communication to be availiable
            between main() and execute(), but can present issues of concurrency on multiple threads if more
            than one file includes the data. Semaphores or mutexes might be required in this case.

    Design Issue 3: Maintainable Flow of Instruction Fetching --> Execution of an Assembly Program
        Solution 1:
            The fetching of instructions ought to read from a MASM-like file line-by-line.
            Aside from the header, each line should constitute a single instruction.
            Fetch the instructions and feed them into the parser, one-by-one. The parser should
            feed into the decoder, and the decoder should feed into a data structure designed to hold the
            instructions.

            The data structure ought to be a dynamically-allocated array of Instruction structs.
            We want an array, since we need random access of some sort to allow for branching, jumping,
            and function calling (should we add that in the future). This rules out linked list (too slow).

            Once the WHOLE file is read and all instructions put into our "instruction memory"
            data structure, the execution can begin. We cannot let the decoder feed directly into the executer,
            since it is possible that the assembly program will have an instruction that results in jumping
            ahead of whatever instruction the decoder is currently processing. Every instruction must be known
            before execution can proceed.

    Design Issue 4: Dynamic Instruction Memory
        Solution 1: Dynamic Allocation of Blocks, with Blocks of a Constant Size:
            To allow for a file of any size to be read, we can organize instructions like so:
                First, define the block size (say, 2048 instructions)
                Then, define a linked list of blocks. Start with just 1.
                When the first block is full, malloc a second block.
                Allocate more blocks as needed.

                This should work, but jumping will require slightly more work.
                For instance, if an instruction at index 1000 jumps to 3000, the memory
                file must goto the second block to find the instruction. This is simply
                a matter of performing "index % 2048" to find the block number.
                This is much more efficient, even if random access is only possible
                within blocks.

                Notice that this bares similarities to file system drivers in regards
                to how blocks are allocated for files of varying length.

    Design Issue 5: Getting rid of the Java naming standards that have invaded my C standards.
        Solution 1: Instruction = instn, Instruction Fetch = instn_fetch, replace caps with underscoring.
*/