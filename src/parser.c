#include <string.h>
#include <regex.h>
#include "parser.h"
#include <stdio.h>

struct parser *create_parser()
{
    struct parser *parser = malloc(sizeof(struct parser));

    if (parser != NULL)
    {
        COMMAND *north = create_command("North", "FILL IN DESC", "^\\ *(north|n)\\ *$", 0);
        COMMAND *south = create_command("South", "FILL IN DESC", "^\\ *(south|s)\\ *$", 0);
        COMMAND *east = create_command("East", "FILL IN DESC", "^\\ *(east|s)\\ *$", 0);
        COMMAND *west = create_command("West", "FILL IN DESC", "^\\ *(west|s)\\ *$", 0);
        COMMAND *look = create_command("Look", "FILL IN DESC", "^\\ *look\\ *$", 0);

        COMMAND *take = create_command("Take", "FILL IN DESC", "^\\ *take\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);
        COMMAND *drop = create_command("Drop", "FILL IN DESC", "^\\ *drop\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);
        COMMAND *use = create_command("Use", "FILL IN DESC", "^\\ *use\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);
        COMMAND *examine = create_command("Examine", "FILL IN DESC", "^\\ *examine\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);

        COMMAND *inventory = create_command("Inventory", "FILL IN DESC", "^\\ *(inventory|i)\\ *$", 0);

        COMMAND *restart = create_command("Restart", "FILL IN DESC", "^\\ *restart\\ *$", 0);
        COMMAND *quit = create_command("Quit", "FILL IN DESC", "^\\ *(quit|exit|q)\\ *$", 0);
        COMMAND *load = create_command("Load", "FILL IN DESC", "^\\ *load\\ *$", 0);
        COMMAND *save = create_command("Save", "FILL IN DESC", "^\\ *save\\ *$", 0);
        COMMAND *version = create_command("Version", "FILL IN DESC", "^\\ *version\\ *$", 0);
        COMMAND *about = create_command("About", "FILL IN DESC", "^\\ *about\\ *$", 0);
        COMMAND *help = create_command("Help", "FILL IN DESC", "^\\ *(help|h)\\ *$", 0);

        struct container *commands = create_container(NULL, TYPE_COMMAND, north);
        commands = create_container(commands, TYPE_COMMAND, south);
        commands = create_container(commands, TYPE_COMMAND, east);
        commands = create_container(commands, TYPE_COMMAND, west);
        commands = create_container(commands, TYPE_COMMAND, look);
        commands = create_container(commands, TYPE_COMMAND, take);
        commands = create_container(commands, TYPE_COMMAND, drop);
        commands = create_container(commands, TYPE_COMMAND, use);
        commands = create_container(commands, TYPE_COMMAND, examine);
        commands = create_container(commands, TYPE_COMMAND, inventory);

        commands = create_container(commands, TYPE_COMMAND, restart);
        commands = create_container(commands, TYPE_COMMAND, quit);
        commands = create_container(commands, TYPE_COMMAND, load);
        commands = create_container(commands, TYPE_COMMAND, save);
        commands = create_container(commands, TYPE_COMMAND, version);
        commands = create_container(commands, TYPE_COMMAND, about);
        commands = create_container(commands, TYPE_COMMAND, help);

        parser->commands = commands;
        parser->history = NULL;
        return parser;
    }

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
                    free(command->groups[i-1]);
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