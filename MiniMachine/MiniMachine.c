// MiniMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "token.h"
#include "instructionDecode.h"

int main()
{
    char* bigstring = "ABCDEF";
    char* substring = NULL;
    struct Token* mytoken = NULL;
    struct Token* current = NULL;
    int loopIndex = 0;

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

    substring = slice(bigstring, 2, strlen(bigstring));
    printf("Substring: %s\n", substring);
}


/*
DESIGN CONSIDERATIONS:
    Design Issue 1: Scope of stack, SP, PC, register file, instruction memory:
        Solution 1:
            Define these in main(). Define struct holding pointers to all globally availiable points of data.
            Pass this struct to relevant functions (ie. instruction execution functions).
        Solution 2:
            Define these in a separate file. Allow others files to access static variables. Encapsulate to ensure
            security (ie. do NOT make global variables for stack, SP, etc.).
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
*/