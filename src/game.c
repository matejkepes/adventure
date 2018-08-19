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
    if (game == NULL || command == NULL)
        return;

    if (strcmp(command->name, "North") == 0)
    {
    }

    if (strcmp(command->name, "South") == 0)
    {
    }

    if (strcmp(command->name, "East") == 0)
    {
    }

    if (strcmp(command->name, "West") == 0)
    {
    }

    if (strcmp(command->name, "Look") == 0)
    {
        show_room(game->current_room);
    }

    if (strcmp(command->name, "Take") == 0)
    {
        struct container *item_to_take = get_from_container_by_name(game->current_room->items, command->groups[0]);
    
        if (item_to_take == NULL)
            ("There is no such item in this room.\n");
        
        if (item_to_take->item->properties != MOVABLE)
            ("This item is too heavy to be picked up.\n");

        if (game->backpack->size == game->backpack->capacity)
            ("There is not enough room in your backpack for this item. You will have to drop something else first.\n");
    
        game->backpack = item_to_take;
        game->backpack->size--;
        game->current_room->items = remove_container(game->current_room->items, item_to_take);
        printf("You have taken %s.\n", item_to_take);
    }

    if (strcmp(command->name, "Drop") == 0)
    {
        struct container *item_to_drop = get_from_container_by_name(game->backpack->items, command->groups);
    }

    if (strcmp(command->name, "Use") == 0)
    {
    }

    if (strcmp(command->name, "Examine") == 0)
    {
    }

    if (strcmp(command->name, "Inventory") == 0)
    {
    }

    if (strcmp(command->name, "Restart") == 0)
    {
    }

    if (strcmp(command->name, "Quit") == 0)
    {
    }

    if (strcmp(command->name, "Load") == 0)
    {
    }

    if (strcmp(command->name, "Save") == 0)
    {
    }

    if (strcmp(command->name, "Version") == 0)
    {
    }

    if (strcmp(command->name, "About") == 0)
    {
    }

    if (strcmp(command->name, "Help") == 0)
    {
    }
}