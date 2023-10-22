// MiniMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
//#include "token.h"
#include "instructionDecode.h"
#include "instructionExecute.h"
#include "instructionFetch.h"
#include "instructionset.h"
#include "instructiondata.h"
#include "memoryfile.h"
//#include "wordmatch.h"

int main()
{
    int debug_counter = 0;
    char* instn_line = NULL;
    struct Instruction* instn = NULL;

    // Initialize globals
    initialize(2048, 16);

    // Load instruction set
    loadinstructionset_full();

    // Fetch instructions
    open_file_read("example.asm");
    while (1) {
        instn_line = fetchinstruction();
        if (instn_line == NULL) {
            break;
        }

        if (strlen(instn_line) > 0) {
            // Decode instruction, add to memory
            addinstruction(decodeline(instn_line));
        }
        //else {
        //    break;
        //}


        debug_counter++;
    }
    close_file();

    printf("\n");
    // Read instructions from instn memory
    setpc(0);
    begin_execution();

    printf("\n");
    printf("\n");

    for (int i = 0; i < 16; i++) {
        printf("Register %d value: %d\n", i, getreg(i));
    }
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

    Design Issue 6: Organizing the Instruction Set
        Solution 1:
            Package execution functions with files defining the instruction set.
            Have "instructionset.h" define all types, instruction data structures
            Have "instructiondata.h" and "instructiondata.c" be used for the sole purpose of loading the set to wordmatch.
            Define a function in "instructiondata.c" that loads everything to wordmatch.
            Move the struct definitions currently in "instructiondata.h" to "instructiondata.c".
            "instructionexecute.c" defines functions executing instructions. It should NOT play with "instructiondata.h", it only needs "instructionset.h".
*/