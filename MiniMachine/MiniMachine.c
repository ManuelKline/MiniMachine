// MiniMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdio.h"
#include "token.h"
#include "instructionDecode.h"

int main()
{
    char* bigstring = "ABC";
    char* substring = NULL;
    struct Token* mytoken = NULL;
    struct Token* current = NULL;
    int loopIndex = 0;

    mytoken = tokenize("ADD 3 2 1");
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

    substring = slice(bigstring, 0, 4);
    printf("Substring: %s\n", substring);
}
