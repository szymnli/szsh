#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "execute.h"
#include "parse.h"

void execute(char **args){
    pid_t pid = fork();
    if (-1 == pid){
        perror("fork failed");
        exit(1);
    }
    // Child process
    else if (0 == pid){
        signal(SIGINT, SIG_DFL); // restore default behavior
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }
    // Parent process (pid > 0)
    else {
        waitpid(pid, NULL, 0);
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
        signal(SIGINT, SIG_DFL); // restore default behavior
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
        signal(SIGINT, SIG_DFL); // restore default behavior
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
