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

            if (command->nmatch > 0)
            {

                for (int i = 1; i < command->nmatch + 1; i++)
                {
                    // puts cursor at the beginning of the first match
                    char *cursor = input + groupArray[i].rm_so;

                    // koniec matchu - zaciatok matchu = dlzka matchu
                    int string_size = (groupArray[i].rm_eo - groupArray[i].rm_so);

                    // allocate memory for match size into parameter
                    char *parameter = malloc(sizeof(char) * (string_size + 1));
                    memcpy(parameter, cursor, string_size);

                    parameter[string_size] = '\0';

                    command->groups[i - 1] = parameter;
                }
            }

            return (struct command *)command;
        }
    }

    return NULL;
}