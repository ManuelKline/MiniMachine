#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define ALPHABET_LENGTH 26
#define MAX_WORD_LENGTH 64

#define A_BIT 1
#define B_BIT 2
#define C_BIT 4
#define D_BIT 8
#define E_BIT 16
#define F_BIT 32
#define G_BIT 64
#define H_BIT 128
#define I_BIT 256
#define J_BIT 512
#define K_BIT 1024
#define L_BIT 2048
#define M_BIT 4096
#define N_BIT 8192
#define O_BIT 16384
#define P_BIT 32768
#define Q_BIT 65536
#define R_BIT 131072
#define S_BIT 262144
#define T_BIT 524288
#define U_BIT 1048576
#define V_BIT 2097152
#define W_BIT 4194304
#define X_BIT 8388608
#define Y_BIT 16777216
#define Z_BIT 33554432
#define VALID_BIT 67108864
// 27/32 bits used

// Error codes:
#define ERR_NOT_ALPHA 1
#define ERR_TOO_LONG 2
#define ERR_NOT_FOUND 3
#define ERR_NULL_PTR 4

// Warning codes:
#define WARN_CAP_FOUND 11

// Important ASCII Values:
#define ASCII_A_CAP 65
#define ASCII_Z_CAP 90
#define ASCII_A 97
#define ASCII_Z 122
#define ASCII_DECAP_VAL 32

// Indexed array to each bit, serves as a conveinent map for ASCII-to-bit conversion
const static unsigned long int AlphaMap[] = {
    A_BIT,
    B_BIT,
    C_BIT,
    D_BIT,
    E_BIT,
    F_BIT,
    G_BIT,
    H_BIT,
    I_BIT,
    J_BIT,
    K_BIT,
    L_BIT,
    M_BIT,
    N_BIT,
    O_BIT,
    P_BIT,
    Q_BIT,
    R_BIT,
    S_BIT,
    T_BIT,
    U_BIT,
    V_BIT,
    W_BIT,
    X_BIT,
    Y_BIT,
    Z_BIT
};

// Node data strcuture:
struct Node {
    unsigned long int validity;             // 26 bits represent valid next nodes - remaining 6 bits indicate validity (000001...b = valid)
    int wordId;                             // Word ID number in dictionary, -1 = does not exist/removed
    struct Node* children[ALPHABET_LENGTH]; // Pointers to children nodes, only some are valid
};

// Root node
static struct Node root = { 0, -1, { 0 } };

// Dictionary data structure (linked list):
struct DictEntry {
    unsigned int wordNum;           // Placement of this word in the list by index, used for identification.
    unsigned char exists;           // The validity of this word - can be set to 0 if removed, else it is 1.
    char* word;                     // The word itself.
    struct DictEntry* nextEntry;    // The next dictionary entry.
};

static struct DictEntry* dictRoot = NULL;

// General error output function.
// Good source I found for printing function names and line numbers is here:
// https://stackoverflow.com/questions/8884335/print-the-file-name-line-number-and-function-name-of-a-calling-function-c-pro
void print_error(const int code, const char* function, const int linenum) {
    printf("Error in %s at line %d, code %d: ", function, linenum, code);

    switch (code) {
    case ERR_NOT_ALPHA:
        printf("Character is non-alpha.\n");
        break;
    case ERR_TOO_LONG:
        printf("Word length exceeds 64 characters.\n");
        break;
    case ERR_NOT_FOUND:
        printf("Word was not found.\n");
        break;
    case ERR_NULL_PTR:
        printf("A null pointer was passed to the function\n.");
        break;
    }
}

/*  FUNCTION:
        dict_append(char* word)
    ARGUMENTS:
        Pointer to char of word to be added to dictionary.
    RETURN VALUES:
        wordNumVal: The ID number of the word added, based on its position in the dictionary.
            This ID number should be stored by the caller for later use.
            This ID number is required for the removal of the word.
    HIGH-LEVEL DESCRIPTION:
        The function creates a new dictionary if empty, else appends to the dictionary if it exists.
        The function then searches for the next free entry in the dictionary from start.
        When found, the function erases the contents of the free entry and adds the new word in.
*/
int dict_append(char* word) {
    unsigned int wordNumVal = 0;
    unsigned int loopIndex = 0;
    struct DictEntry* currentEntry = NULL;

    // If dictionary is empty, create first entry
    if (dictRoot == NULL) {
        dictRoot = (struct DictEntry*)calloc(1, sizeof(struct DictEntry));
        dictRoot->wordNum = loopIndex;
        dictRoot->exists = 0;
        dictRoot->word = NULL;
        dictRoot->nextEntry = NULL;
    }
    currentEntry = dictRoot;

    while (1) {
        // If current entry is free:
        if (!currentEntry->exists) {
            // If there exists a word in this entry
            if (currentEntry->word != NULL) {
                // Delete it
                free(currentEntry->word);
            }
            // Allocate memory for new word and copy
            size_t length = strlen(word) + 1;
            currentEntry->word = (char*)calloc(1, length);
            strcpy_s(currentEntry->word, length, word);
            // Declare that is exists
            currentEntry->exists = 1;
            // Finish loop
            wordNumVal = loopIndex;
            break;
        }
        else {
            // If next entry doesn't exist
            if (currentEntry->nextEntry == NULL) {
                // Create the next entry
                currentEntry->nextEntry = (struct DictEntry*)calloc(1, sizeof(struct DictEntry));
                // Set defaults, let wordNum = current loop iteration
                currentEntry->nextEntry->wordNum = loopIndex + 1;
                currentEntry->nextEntry->exists = 0;
                currentEntry->nextEntry->word = NULL;
                currentEntry->nextEntry->nextEntry = NULL;
            }
            // Continue loop with next entry
            currentEntry = currentEntry->nextEntry;
        }
        loopIndex++;
    }

    return wordNumVal;
}

/*  FUNCTION:
        dict_remove(int wordIdVal)
    ARGUMENTS:
        Integer value of the word ID number, created during the append process.
    RETURN VALUES:
        0 = No error
        >0 = Error (error code enumerated in header)
    HIGH-LEVEL DESCRIPTION:
        The function searches the dictionary for the word to be removed based on ID number.
        When the matching ID is found, the word is erased and the entry is marked as free.
        The function returns an error if the word is not found, or if the dictionary is empty.
*/
int dict_remove(int wordIdVal) {
    struct DictEntry* currentEntry = NULL;
    int returnVal = 0;

    // If dictionary is empty, return error
    if (dictRoot == NULL) {
        returnVal = ERR_NOT_FOUND;
    }
    else {
        currentEntry = dictRoot;

        while (1) {
            // If current entry contains matching word ID num
            if (currentEntry->wordNum == wordIdVal) {
                // Set non-existant, and end
                currentEntry->exists = 0;
                //printf("Removal success\n");
                break;
            }
            else {
                // Else, attempt to move to next entry
                if (currentEntry->nextEntry != NULL) {
                    currentEntry = currentEntry->nextEntry;
                }
                else {
                    // If failed, return error
                    returnVal = ERR_NOT_FOUND;
                    break;
                }
            }
        }
    }

    return returnVal;
}

/*  FUNCTION:
        word_verify(char* word)
    ARGUMENTS:
        Pointer to char of word to verify.
    RETURN VALUES:
        0 = No error
        >0 = Error (error code enumerated in header)
    HIGH-LEVEL DESCRIPTION:
        The function analyzes the string for characters that are not allowed, as well as
        excessive length. If the string is too long, has non-alpha characters, or the argument
        is a null pointer, then an error is returned.
*/
int word_verify(char* word) {
    int returnVal = 0;

    if (!word) {
        returnVal = ERR_NULL_PTR;
    }

    if (word && (strlen(word) > MAX_WORD_LENGTH)) {
        returnVal = ERR_TOO_LONG;
    }

    if (word) {
        for (unsigned int i = 0; i < strlen(word); i++) {
            // If char is not in range a:z
            if (word[i] < ASCII_A || word[i] > ASCII_Z) {
                // If char is not in range A:Z
                if (word[i] < ASCII_A_CAP || word[i] > ASCII_Z_CAP) {
                    returnVal = ERR_NOT_ALPHA;
                }
                //else {
                //    returnVal = WARN_CAP_FOUND;
                //}
            }
        }
    }

    return returnVal;
}

/*  FUNCTION:
        word_decap(char* word)
    ARGUMENTS:
        Pointer to char of word to decapitalize.
    RETURN VALUES:
        Pointer to decapitalized word.
    HIGH-LEVEL DESCRIPTION:
        The function decapitalizes alpha characters in a word, and returns the result.
        This is to be used after verification of the word.
*/
char* word_decap(char* word) {
    char* newString = NULL;
    size_t length = strlen(word);
    //printf("Malloc size: %d\n", sizeof(char*));
    newString = malloc(length + 1);
    //printf("Parameter: %s\n", word);
    ////printf("Length of Parameter: %d\n", strlen(word));
    //printf("Size of Parameter: %d\n", sizeof(word));
    //printf("Length of newString: %d\n", strlen(newString));
    // Copy string

    //printf("Arguments: %s, %d, %s\n", newString, length, word);
    if (newString != NULL) {
        strcpy_s(newString, length + 1, word);
    }
    else {
        return NULL;
    }

    for (unsigned int i = 0; i < length; i++) {
        // If char is not in range a:z
        if (word[i] <= ASCII_A || word[i] >= ASCII_Z) {
            // If char is in range A:Z
            if (word[i] >= ASCII_A_CAP && word[i] <= ASCII_Z_CAP) {
                newString[i] = newString[i] + ASCII_DECAP_VAL;
            }
        }
    }

    if (newString) {
        return newString;
    }
    else {
        return NULL;
    }
}

// ALL FUNCTIONS BELOW CAN BE CALLED BY MAIN

/*  FUNCTION:
        find_word(char* word)
    ARGUMENTS:
        Pointer to char of word to find in tree.
    RETURN VALUES:
        1 if word is found.
        0 if not found.
    HIGH-LEVEL DESCRIPTION:
        The function searches the tree for the matching word, and stops on the following conditions:
        Firstly, the sequence of letters must exist.
        Secondly, the last letter must also contain a valid bit.
        Both conditions must be true for the word to exist.

        The function does not use the dictionary - the tree data structure is more efficient.
        It also uses bitwise comparisons to find letters - something which is computationally efficient.
        The whole rationale of this function is to be fast - the dictionary exists for other purposes.
*/
int find_word(char* word) {
    struct Node* current = &root;
    int returnVal = 1;
    int err;

    // Verify word before use
    err = word_verify(word);
    if (err) {
        print_error(err, __func__, __LINE__);
        return;
    }

    // Decapitalize word after being verified for alpha only chars.
    char* word_safe = word_decap(word);
    //printf("Find word new parameter: %s\n", word_safe);

    for (unsigned int i = 0; i < strlen(word_safe); i++) {
        char letter = word_safe[i] - 'a';    // Index of letter in alphabet, normalized
        if (current->validity & AlphaMap[letter]) { // If valid bit exists for this letter
            current = current->children[letter];    // Jump to next node
        }
        else {  // Else, the word does not exist
            returnVal = 0;
            break;
        }
    }

    // if search ended but current node is NOT valid:
    if (!(current->validity & VALID_BIT) && returnVal) {
        returnVal = 0;
    }

    return returnVal;
}

/*  FUNCTION:
        add_word(char* word)
    ARGUMENTS:
        Pointer to char of word to add to tree and dictionary.
    RETURN VALUES:
        None.
    HIGH-LEVEL DESCRIPTION:
        The function adds a word to the tree, and subsequently the dictionary.
        It creates new nodes as needed based on whether or not the sequence of letters
        already exists. Before doing so, it verifies that the word is composed of only
        alpha characters. It also decapitalizes the word.
*/
void add_word(char* word) {
    struct Node* current = &root;
    int err;

    err = word_verify(word);
    if (err) {
        print_error(err, __func__, __LINE__);
        return;
    }

    char* word_safe = word_decap(word);

    for (unsigned int i = 0; i < strlen(word_safe); i++) {
        char letter = word_safe[i] - 'a';    // Index of letter in alphabet, normalized
        //printf("Loop Index: %d\n", i);
        //printf("Validity Initial: %lu\n", current->validity);
        //current->validity = current->validity | AlphaMap[letter];   // Set valid bit for corresponding letter
        //printf("Validity Final: %lu\n", current->validity);
        if ((current->validity & AlphaMap[letter]) && current->children[letter]) {    // If next node already exists
            //printf("Node has children, progressing\n");
            current->validity = current->validity | AlphaMap[letter];
            //printf("Validity Final: %lu\n", current->validity);
            current = current->children[letter];
        }
        else {  // Else, create new node
            //printf("Node lacks child, allocating\n");
            current->validity = current->validity | AlphaMap[letter];
            //printf("Validity Final: %lu\n", current->validity);
            // Use calloc to zero all allocated memory
            current->children[letter] = (struct Node*)calloc(1, sizeof(struct Node));
            current = current->children[letter];
            // Set validity to default
            current->validity = 0;
            current->wordId = -1;
        }
    }

    // When finished, set valid bit on last node
    current->validity = current->validity | VALID_BIT;

    // Add this word to the dictionary
    current->wordId = dict_append(word_safe);
}

/*  FUNCTION:
        remove_word(char* word)
    ARGUMENTS:
        Pointer to char of word to find and remove in tree.
    RETURN VALUES:
        1 if word is found.
        0 if not found.
    HIGH-LEVEL DESCRIPTION:
        The function is quite similar to find_word, except it sets the valid bit to zero.
        Deletion of nodes would be painful here, so that is left up to the free_all function.
*/
int remove_word(char* word) {
    struct Node* current = &root;
    int returnVal = 1;  // Remains 1 if word successfully found and removed
    int err;

    err = word_verify(word);
    if (err) {
        print_error(err, __func__, __LINE__);
        return;
    }

    char* word_safe = word_decap(word);

    // Similar procedure to finding word, except now we are invalidating it
    for (unsigned int i = 0; i < strlen(word_safe); i++) {
        char letter = word_safe[i] - 'a';    // Index of letter in alphabet, normalized
        if (current->validity & AlphaMap[letter]) { // If valid bit exists for this letter
            current = current->children[letter];    // Jump to next node
        }
        else {  // Else, the word does not exist
            returnVal = 0;
            break;
        }
    }

    // if search ended but current node is NOT valid:
    if (!(current->validity & VALID_BIT) && returnVal) {
        returnVal = 0;
    }
    else {  // Else, search found word
        // Set valid bit to zero
        current->validity = current->validity ^ VALID_BIT;
    }

    dict_remove(current->wordId);

    return returnVal;
}

// TODO Functions (of varying complexity and difficulty):

// save_dict: Saves all words to a file for later usage
void save_dict(const char* filename) {

}

// load_dict: Loads all words from a file and creates the tree
void load_dict(const char* filename) {

}

// print_dict: Simply prints the whole dictionary
void print_dict() {
    struct DictEntry* current;

    printf("Dictionary:\n");
    if (dictRoot != NULL) {
        current = dictRoot;
        while (1) {
            if (current->exists) {
                printf("Entry %d: %s\n", current->wordNum, current->word);
            }
            if (current->nextEntry != NULL) {
                current = current->nextEntry;
            }
            else {
                printf("End of dictionary\n");
                break;
            }
        }
    }
}


// Recursive function for freeing tree, should begin with root
void free_tree(struct Node* node) {
    //printf("free_tree: Validity: %lu, Word ID: %d\n", node->validity, node->wordId);
    for (unsigned int i = 0; i < ALPHABET_LENGTH; i++) {
        if (node->validity & AlphaMap[i]) {
            //printf("Attempting to call free_tree on child %d\n", i);
            free_tree(node->children[i]);
        }
    }

    if (node == &root) {
        //printf("Returned to root, no free.\n");
        return;
    }

    //printf("Attempting to free: Validity: %lu, Word ID: %d\n", node->validity, node->wordId);
    free(node);
    return;
}

// Recursive function for freeing dictionary, should begin with first entry
void free_dict(struct DictEntry* entry) {
    // Go to next node if it exists (should take thread to the end of the dictionary)
    if (entry->nextEntry != NULL) {
        free_dict(entry->nextEntry);
    }

    // If there is a word in this entry, free it
    if (entry->word != NULL) {
        free(entry->word);
    }

    // Free this entry completely
    free(entry);
    return;
}

// free_all: Frees all dynamically allocated memory, mainly the tree and the dictionary
void free_all() {
    free_tree(&root);
    printf("Tree freed\n");
    free_dict(dictRoot);
    printf("Dict freed\n");
}

// tree_test: Tests the tree to ensure consistency between the tree and dictionary.
void tree_test() {
    struct DictEntry* current;

    printf("Dictionary:\n");
    if (dictRoot != NULL) {
        current = dictRoot;
        while (1) {
            if (current->exists) {
                printf("Entry %d: %s\n", current->wordNum, current->word);
                if (find_word(current->word)) {
                    printf("   Word exists in tree\n");
                }
                else {
                    printf("   FAIL: Word not found in tree\n");
                }
            }
            if (current->nextEntry != NULL) {
                current = current->nextEntry;
            }
            else {
                printf("End of dictionary\n");
                break;
            }
        }
    }
}
