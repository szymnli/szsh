#include <stdio.h>
#include <string.h>

char **parse(char *input){
    char **fn_list;
    return fn_list;
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
        }
    }

    return 0;
}
