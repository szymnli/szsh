#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "builtin.h"

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
