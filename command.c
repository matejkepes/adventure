#include "command.h"
#include <stdlib.h>
#include <string.h>

struct command *create_command (char *name, char *description, char *pattern, size_t nmatch) {
    //Alloc for command struct
    struct command *command = malloc(sizeof(struct command));
   
    //Alloc for command->name
    command->name = strdup(name);
   
    //Alloc for command->description
    command->description = strdup(description);
   
    //Compile regex_t preg
    int compilation_result = regcomp(&(command->preg), pattern, 0);
    if (compilation_result) {
        printf("Could not compile regex\n");
        exit(1);
    }   

    return command;
}

struct command *destroy_command (struct command *command) {
    //free name
    free(command->name);
    
    //free description
    free(command->description);

    //regfree preg
    regfree(&(command->preg));

    //free all groups
    for (int i=0; i<command->nmatch; i++) {
        free(command->groups[i]);
    }

    free(command->groups);
}