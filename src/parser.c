#include <string.h>
#include <regex.h>
#include "parser.h"

struct parser *create_parser()
{
    // struct container *commands = create_command("look", "scan your environment for objects", "/(look)/gi", );

    struct parser *parser = malloc(sizeof(struct parser));

    if (parser != NULL)
        return parser;

    return NULL;
}

struct parser *destroy_parser(struct parser *parser)
{
    if (parser != NULL) {
        parser = destroy_containers(parser->commands);
        parser = destroy_containers(parser->history);
        parser = NULL;
    }

    return NULL;
}

struct command *parse_input(struct parser *parser, char *input)
{

}