#include <stdlib.h>
#include <string.h>
#include "parse.h"

char **parse(char *input){
    // allocate array of string pointers
    char **args = malloc(MAX_ARGS * sizeof(char *));
    int i = 0;

    char *token = strtok(input, " ");
    while (token != NULL){
        args[i] = token; // store pointer to this toke
        i++;
        token = strtok(NULL, " "); // get next token
    }
    args[i] = NULL; // terminate with NULL
    return args;
}
