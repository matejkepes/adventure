#include <string.h>
#include <regex.h>
#include "parser.h"
#include <stdio.h>

struct parser *create_parser()
{
    struct parser *parser = malloc(sizeof(struct parser));

    if (parser != NULL)
    {
        COMMAND *north = create_command("North", "Takes your character to the north.", "^\\ *(north|n)\\ *$", 0);
        COMMAND *south = create_command("South", "Takes your character to the south.", "^\\ *(south|s)\\ *$", 0);
        COMMAND *east = create_command("East", "Takes your character to the east.", "^\\ *(east|s)\\ *$", 0);
        COMMAND *west = create_command("West", "Takes your character to the west.", "^\\ *(west|s)\\ *$", 0);
        COMMAND *look = create_command("Look", "Gives you a full description of your location.", "^\\ *(look|l)\\ *$", 0);

        COMMAND *take = create_command("Take", "Puts the specified item from the current room into your backpack.", "^\\ *(take|t)\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);
        COMMAND *drop = create_command("Drop", "Drops the specified item from your backpack into the current room.", "^\\ *(drop|d)\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);
        COMMAND *use = create_command("Use", "Uses the specified item.", "^\\ *(use|u)\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);
        COMMAND *examine = create_command("Examine", "Gives you a full description of the specified item.", "^\\ *(examine|e)\\ +([a-z0-9][a-z0-9\\ ]*[a-z0-9])\\ *$", 1);

        COMMAND *inventory = create_command("Inventory", "Shows you the items you are currently holding in your backpack.", "^\\ *(inventory|i)\\ *$", 0);

        COMMAND *restart = create_command("Restart", "Starts the story over from the beginning.", "^\\ *(restart|r)\\ *$", 0);
        COMMAND *quit = create_command("Quit", "Quits the game. Make sure to save your game before quitting.", "^\\ *(quit|exit|q)\\ *$", 0);
        COMMAND *load = create_command("Load", "Loads the most recently saved game.", "^\\ *(load|restore)\\ *$", 0);
        COMMAND *save = create_command("Save", "Saves the game. You can return to the game by using the LOAD command.", "^\\ *save\\ *$", 0);
        COMMAND *version = create_command("Version", "Tells you the version of the game.", "^\\ *version\\ *$", 0);
        COMMAND *about = create_command("About", "Tells you about the story of the game.", "^\\ *about\\ *$", 0);
        COMMAND *help = create_command("Help", "Gives you a list of all available commands.", "^\\ *(help|h)\\ *$", 0);

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
        parser->commands = destroy_containers(parser->commands);
        parser->history = destroy_containers(parser->history);
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