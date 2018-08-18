#include <string.h>
#include <regex.h>
#include "parser.h"
#include <stdio.h>

struct parser *create_parser()
{
    // struct container *commands = create_command("look", "scan your environment for objects", "/look/gi", );

    struct parser *parser = malloc(sizeof(struct parser));

    if (parser != NULL)
        return parser;

    return NULL;
}

struct parser *destroy_parser(struct parser *parser)
{
    if (parser != NULL)
    {
        destroy_containers(parser->commands);
        destroy_containers(parser->history);
        parser = NULL;
    }

    return NULL;
}

struct command *parse_input(struct parser *parser, char *input)
{

    // "pickup MIGHTY SWORD"

    if (parser == NULL || input == NULL)
        return NULL;

    int result;

    for (struct container *head = parser->commands; head; head = head->next)
    {
        COMMAND *command = head->command;
        regmatch_t groupArray[command->nmatch + 1];

        result =
            command->nmatch == 0 ? regexec(&(command->preg), input, 0, NULL, 0)
                                 : regexec(&(command->preg), input, command->nmatch + 1, groupArray, 0);

        if (result == 0)
        {
            printf("INPUT: %s MATCH: %s\n", input,command->name);
            if (command->nmatch > 0)
            {
                char * cursor = input + groupArray[1].rm_so;

                int string_size = (groupArray[1].rm_eo - groupArray[1].rm_so);
                char parameter[string_size + 1];

                memcpy(parameter, cursor, string_size);

                parameter[string_size] = '\0';

                printf("S: %lld E: %lld Param: %s\n", groupArray[1].rm_so, groupArray[1].rm_eo, parameter);

               command->groups[0]=parameter;
            }

            return (struct command *)command;
        }
        //if result == 0
        //int startOffset, int endOffset
        //input[endOffset] = '\0'
        //
        //char * param = strdup(input[startOffset])
        //head->command->groups[0] = param;
        //return head->command;
    }

    return NULL;
}