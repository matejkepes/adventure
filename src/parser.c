#include <string.h>
#include <regex.h>
#include "parser.h"

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
    if (parser != NULL) {
        destroy_containers(parser->commands);
        destroy_containers(parser->history);
        parser = NULL;
    }

    return NULL;
}

struct command * parse_input(struct parser *parser, char *input)
{

    // "pickup MIGHTY SWORD"

    if(parser == NULL|| input == NULL)
        return NULL;

    int result;
    
    for(struct container * head = parser->commands; head; head = head->next) {
        regmatch_t groupArray[head->command->nmatch];

        result = regexec(&(head->command->preg), input, head->command->nmatch, groupArray, 0);

        if(result == 0) {
            return head->command;
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