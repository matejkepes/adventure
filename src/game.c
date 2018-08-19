#include "game.h"
#include <stdio.h>

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
        // check for matches
        if (!command->groups[0])
            printf("What do you want to take?\n");

        // find the item in the room
        struct container *item_to_take = get_from_container_by_name(game->current_room->items, command->groups[0]);

        // was the item found?
        if (!item_to_take)
            printf("There is no %s in this room.\n", command->groups[0]);

        // can you pick up the item?
        if (item_to_take->item->properties != MOVABLE)
            printf("This item is too heavy to be picked up.\n");

        // is there enough room in your backpack?
        if (game->backpack->size == game->backpack->capacity)
            printf("There is not enough room in your backpack for the %s. You will have to drop something else first.\n", item_to_take->item->name);

        // take the item
        game->backpack->items = item_to_take;
        game->backpack->size--;
        game->current_room->items = remove_container(game->current_room->items, item_to_take);
        printf("You have taken %s.\n", item_to_take->item->name);
    }

    if (strcmp(command->name, "Drop") == 0)
    {
        // check for matches
        if (!command->groups[0])
            printf("What do you want to drop?\n");

        // find the item in the backpack
        struct container *item_to_drop = get_from_container_by_name(game->backpack->items, command->groups[0]);

        // was the item found?
        if (!item_to_drop)
            printf("You can't drop %s.\n", command->groups[0]);

        // drop the item
        game->current_room->items = create_container(game->current_room->items, TYPE_ITEM, item_to_drop);
        game->backpack->items = remove_container(game->backpack->items, item_to_drop);
        game->backpack->size++;
        printf("You have dropped %s.\n", item_to_drop->item->name);
    }

    if (strcmp(command->name, "Use") == 0)
    {
        // check for matches
        if (!command->groups[0])
            printf("What do you want to Use?\n");

        // find the item in the room or in the backpack
        struct container *item_to_use;

        // was the item found in the room?
        if (get_from_container_by_name(game->current_room->items, command->groups[0]))
            item_to_use = get_from_container_by_name(game->current_room->items, command->groups[0]);

        // was the item found in the backpack?
        if (get_from_container_by_name(game->backpack->items, command->groups[0]))
            item_to_use = get_from_container_by_name(game->backpack->items, command->groups[0]);

        // was the item not found?
        if (!item_to_use)
            printf("I can't see any %s.\n", command->groups[0]);

        // is the item usable?
        if (item_to_use->item->properties != USABLE)
            printf("You can't use %s.\n", item_to_use->item->name);

        // use the item
        // TODO: implement usability for each item in the game
    }

    if (strcmp(command->name, "Examine") == 0)
    {
        // check for matches
        if (!command->groups[0])
            printf("What do you want to examine?\n");

        // find the item in the room or in the backpack
        struct container *item_to_examine;

        // was the item found in the room?
        if (get_from_container_by_name(game->current_room->items, command->groups[0]))
            item_to_examine = get_from_container_by_name(game->current_room->items, command->groups[0]);

        // was the item found in the backpack?
        if (get_from_container_by_name(game->backpack->items, command->groups[0]))
            item_to_examine = get_from_container_by_name(game->backpack->items, command->groups[0]);

        // was the item not found?
        if (!item_to_examine)
            printf("I can't see any %s.\n", item_to_examine->item->name);

        // is the item examinable?
        if (item_to_examine->item->properties != EXAMINABLE)
            printf("There's nothing to say about %s.\n", item_to_examine->item->name);

        // examine the item
        printf("%s\n.", command->groups[0]);
    }

    if (strcmp(command->name, "Inventory") == 0)
    {
        // is the backpack empty?
        if (game->backpack->size == 0)
            printf("There are no items in your backpack.\n");

        // display the items in your backpack
        if (game->backpack->size > 0)
        {
            printf("These items are in your inventory:\n");
            for (struct container *head = game->backpack->items; head; head = head->next)
                printf("%s\n", head->item->name);
        }
    }

    if (strcmp(command->name, "Restart") == 0)
    {
        char input[20];

        printf("Are you sure you want to restart the game? All progress will be lost.\n");

        scanf("%s", &input);
    
        regex_t yes_reg;
        regex_t no_reg;

        int result =
            regcomp(&yes_reg, "\\ *(y|yes|yep|yeah)\\ *$", REG_ICASE | REG_EXTENDED) &&
            regcomp(&no_reg, "\\ *(n|no|nope|nah)\\ *$", REG_ICASE | REG_EXTENDED);

        if (result == 1)
            exit(1);
        
        int yes = regexec(&yes_reg, input, 0, NULL, 0);
        int no = regexec(&no_reg, input, 0, NULL, 0);

        if (yes && no)
            printf("I didn't understand that.\n");
        
        if (!yes)
            game->state = RESTART;

        if (!no)
            printf("Okay.\n"); 
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