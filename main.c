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

int builtin(char **args){
    if (0 == strcmp(args[0], "cd")){
        if (!args[1]){
            chdir(getenv("HOME"));
        }
        else if(-1 == chdir(args[1])){
            perror("cd failed");
        }
        return 1;
    } else{
        return 0;
    }
}

void execute_pipe(char *input){
    // create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    char *pipe_pos = strchr(input, '|');  // find the | character
    *pipe_pos = '\0';                     // replace | with \0, splitting the string
    char *left = input;                   // "ls "
    char *right = pipe_pos + 1;          // " grep txt"

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // child 1 — will run left command (ls)
        dup2(pipefd[1], 1);   // replace stdout with pipe write end
        close(pipefd[0]);     // child 1 doesn't need read end
        close(pipefd[1]);     // close original write end (dup2 made a copy)
        char **args = parse(left);
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // child 2 — will run right command (grep)
        dup2(pipefd[0], 0);   // replace stdin with pipe read end
        close(pipefd[1]);     // child 2 doesn't need write end
        close(pipefd[0]);     // close original read end (dup2 made a copy)
        char **args = parse(right);
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }

    // parent doesn't use the pipe at all
    close(pipefd[0]);
    close(pipefd[1]);

    // wait for both children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

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
            // Check for pipe
            if (strchr(input, '|')) {
                execute_pipe(input);
            } else {
                char **args = parse(input);
                if (NULL == args[0]) {
                    free(args);
                    continue;
                }
                if (!builtin(args)) {
                    execute(args);
                }
                free(args);
            }
        }
    }

    return 0;
}
