#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parse.h"
#include "execute.h"
#include "builtin.h"

int main(){
    signal(SIGINT, SIG_IGN); // ignore this signal
    char input[1024];
    printf("Type 'exit' to leave the shell\n");
    // Main shell loop
    while(1){
        printf("szsh> ");
        fflush(stdout);
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
