#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 64

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

void execute(char **args){
    pid_t pid = fork();
    if (-1 == pid){
        perror("fork failed");
        exit(1);
    }
    // Child process
    else if (0 == pid){
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }
    // Parent process (pid > 0)
    else {
        waitpid(pid, NULL, 0);
    }
}

int main(){
    char input[1024];
    printf("Type 'exit' to leave the shell\n");
    // Main shell loop
    while(1){
        printf("szsh> ");
        // Error check for fgets
        if (NULL == fgets(input, sizeof(input), stdin)){
            break;
        }
        // Removing the new line at the end of the user's input
        input[strcspn(input, "\n")] = '\0';
        // Stop the shell when the user inputs 'exit'
        if (strcmp("exit", input) == 0){
            break;
        } else {
            char **args = parse(input);
            for (int i = 0; args[i] != NULL; i++){
                printf("%s\n", args[i]);
            }
            execute(args);
            free(args);
        }
    }

    return 0;
}
