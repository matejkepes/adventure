#include "game.h"

void play_game(struct game *game)
{
    if (game != NULL)
    {
        while (game->state == PLAYING)
        {
        }

        if (game->state == SOLVED)
        {
        }

        if (game->state == GAMEOVER)
        {
            game = destroy_game(game);
        }

        if (game->state == RESTART)
        {
            game = destroy_game(game);
            game = create_game();
            game->state = PLAYING;
            play_game(game);
        }
    }
}

struct game *create_game()
{
    struct game *game = malloc(sizeof(struct game));
    game->backpack = create_backpack(BACKPACK_CAPACITY);
    game->current_room = NULL;
    game->parser = create_parser();
    game->state = PLAYING;
    game->world = NULL;

    if (game != NULL)
        return game;

    return NULL;
}

struct game *destroy_game(struct game *game)
{
    if (game != NULL)
    {
        game->backpack = destroy_backpack(game->backpack);
        game->backpack = NULL;
        game->current_room = NULL;
        game->parser = destroy_parser(game->parser);
        game->parser = NULL;
        game->world = destroy_containers(game->world);
        game->world = NULL;
        game->state = GAMEOVER;
    }

    return NULL;
}

void execute_command(struct game *game, struct command *command)
{
}