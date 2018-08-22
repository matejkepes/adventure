#include "game.h"
void play_game(struct game *game)
{
    char input[INPUT_BUFFER_SIZE];
    if (game != NULL)
    {
        printf("\nWelcome to the Adventure!\n\n");
        show_room(game->current_room);

        while (game->state == PLAYING)
        {
            printf("\n> ");
            fgets(input, INPUT_BUFFER_SIZE, stdin);
            input[strlen(input) - 1] = '\0';

            struct command *command = parse_input(game->parser, input);
            if (command != NULL)
            {
                printf("\n");
                execute_command(game, command);
            }
        }

        if (game->state == SOLVED)
        {
            printf("Congratulations!! You have finished the adventure!! What an accomplishment.\n");
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
    printf("Couldn't start the game.\n");
    exit(1);
}

struct game *create_game()
{
    struct game *game = malloc(sizeof(struct game));

    game->world = create_world();
    game->backpack = create_backpack(BACKPACK_CAPACITY);
    game->current_room = game->world->room;
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

    char buffer[100];

    if (strcmp(command->name, "North") == 0 ||
        strcmp(command->name, "South") == 0 ||
        strcmp(command->name, "East") == 0 ||
        strcmp(command->name, "West") == 0 ||
        strcmp(command->name, "Use") == 0 ||
        strcmp(command->name, "Take") == 0 ||
        strcmp(command->name, "Drop") == 0)
    {
        buffer[0] = '\0';
        sprintf(buffer, "%s ", command->name);

        for (int i = 0; i < command->nmatch; i++)
        {
            strcat(buffer, command->groups[i]);
        }
        printf("%s len: %d\n", command->name, strlen(buffer));

        game->parser->history = create_container(game->parser->history, TYPE_TEXT, strdup(buffer));
    }

    //-------------------------------------------------------------------------NORTH--------------------------------

    if (strcmp(command->name, "North") == 0)
    {

        if (game->current_room->north != NULL)
        {
            struct room *room_to_go_to = game->current_room->north;
            game->current_room = room_to_go_to;
            show_room(game->current_room);
        }

        if (game->current_room->north == NULL)
        {
            printf("You can't go there.\n");
            return;
        }
    }

    //-------------------------------------------------------------------------SOUTH--------------------------------

    if (strcmp(command->name, "South") == 0)
    {
        if (game->current_room->south != NULL)
        {
            struct room *room_to_go_to = game->current_room->south;
            game->current_room = room_to_go_to;
            show_room(game->current_room);
            return;
        }

        if (game->current_room->south == NULL)
        {
            printf("You can't go there.\n");
            return;
        }
    }

    //-------------------------------------------------------------------------EAST---------------------------------

    if (strcmp(command->name, "East") == 0)
    {
        if (game->current_room->east != NULL)
        {
            struct room *room_to_go_to = game->current_room->east;
            game->current_room = room_to_go_to;
            show_room(game->current_room);
            return;
        }

        if (game->current_room->east == NULL)
        {
            printf("You can't go there.\n");
            return;
        }
    }

    //-------------------------------------------------------------------------WEST---------------------------------

    if (strcmp(command->name, "West") == 0)
    {
        if (game->current_room->west != NULL)
        {
            struct room *room_to_go_to = game->current_room->west;
            game->current_room = room_to_go_to;
            show_room(game->current_room);
            return;
        }

        if (game->current_room->west == NULL)
        {
            printf("You can't go there.\n");
            return;
        }
    }

    //-------------------------------------------------------------------------LOOK---------------------------------

    if (strcmp(command->name, "Look") == 0)
    {
        show_room(game->current_room);
    }

    //-------------------------------------------------------------------------TAKE---------------------------------

    if (strcmp(command->name, "Take") == 0)
    {
        // check for matches
        if (!command->groups[0])
        {
            printf("What do you want to take?\n");
            return;
        }

        // find the item in the room
        struct container *item_to_take = get_from_container_by_name(game->current_room->items, command->groups[0]);

        // was the item found?
        if (!item_to_take)
        {
            printf("There is no %s in this room.\n", command->groups[0]);
            return;
        }

        // can you pick up the item?
        if (!((item_to_take->item->properties & 0x0001) == 0x0001))
        {
            printf("%s cannot be picked up.\n", item_to_take->item->name);
            return;
        }

        // is there enough room in your backpack?
        if (game->backpack->size == game->backpack->capacity)
        {
            printf("There is not enough room in your backpack for the %s. You will have to drop something else first.\n", item_to_take->item->name);
            return;
        }

        // take the item
        game->backpack->items = create_container(game->backpack->items, TYPE_ITEM, item_to_take->item);
        game->backpack->size++;
        game->current_room->items = remove_container(game->current_room->items, item_to_take->item);
        printf("You have taken %s.\n", item_to_take->item->name);

        // add the executed command to the history log
    }

    //-------------------------------------------------------------------------DROP---------------------------------

    if (strcmp(command->name, "Drop") == 0)
    {
        // check for matches
        if (!command->groups[0])
        {
            printf("What do you want to drop?\n");
            return;
        }

        // find the item in the backpack
        struct container *item_to_drop = get_from_container_by_name(game->backpack->items, command->groups[0]);

        // was the item found?
        if (!item_to_drop)
        {
            printf("You don't have %s in your inventory to drop.\n", command->groups[0]);
            return;
        }

        // drop the item
        game->current_room->items = create_container(game->current_room->items, TYPE_ITEM, item_to_drop->item);
        game->backpack->items = remove_container(game->backpack->items, item_to_drop->item);
        game->backpack->size--;
        printf("You have dropped %s.\n", item_to_drop->item->name);

        // add the executed command to the history log
    }

    //-------------------------------------------------------------------------USE----------------------------------

    if (strcmp(command->name, "Use") == 0)
    {
        // check for matches
        if (!command->groups[0])
        {
            printf("What do you want to use?\n");
            return;
        }

        // find the item in the room or in the backpack
        struct container *item_to_use_room = get_from_container_by_name(game->current_room->items, command->groups[0]);
        struct container *item_to_use_backpack = get_from_container_by_name(game->backpack->items, command->groups[0]);
        struct container *item_to_use;

        // was the item found in the room?
        if (item_to_use_room)
            item_to_use = item_to_use_room;
        // TODO: pick up the item before using it?

        // was the item found in the backpack?
        if (item_to_use_backpack)
            item_to_use = item_to_use_backpack;

        // was the item not found?
        if (!item_to_use)
        {
            printf("I can't see any %s.\n", command->groups[0]);
            return;
        }

        // is the item usable?
        if (!((item_to_use->item->properties & 0x0002) == 0x0002))
        {
            printf("You don't know how to use %s.\n", command->groups[0]);
            return;
        }

        // use the item
        // TODO: implement usability for each item in the game

        // Golden watch will add the bling.
        // Dusty bed will give you rest and allergies.
        // The orb | use it in the last room to win the game.

        if (strcmp(item_to_use->item->name, "Golden Watch") == 0)
        {
            printf("You can feel all the bling pouring over you as you put this watch on your wrist.\n");
        }

        if (strcmp(item_to_use->item->name, "Dusty Bed") == 0)
        {
            printf("You are refreshed after taking a nap in the dusty bed. For some reason, your nose is itching.\n");
        }

        if (strcmp(item_to_use->item->name, "The Orb") == 0)
        {
            if (strcmp(game->current_room->name, "Room O") != 0)
                printf("You put your hand on the orb and notice a slight tingling sensation. The Orb lits up slightly. It's almost as if it was telling you to take it to the last room.\n");

            if (strcmp(game->current_room->name, "Room O") == 0)
            {
                printf("You place your hand on the orb and it lits up the whole room, exposing a writing on the wall which says 'The End'.\n");
                game->state = SOLVED;
            }
        }

        // add the executed command to the history log
    }

    //-------------------------------------------------------------------------EXAMINE------------------------------

    if (strcmp(command->name, "Examine") == 0)
    {
        // check for matches
        if (!command->groups[0])
        {
            printf("What do you want to examine?\n");
            return;
        }

        // find the item in the room or in the backpack
        struct container *item_to_examine_room = get_from_container_by_name(game->current_room->items, command->groups[0]);
        struct container *item_to_examine_backpack = get_from_container_by_name(game->backpack->items, command->groups[0]);
        struct container *item_to_examine;

        // was the item found in the room?
        if (item_to_examine_room)
            item_to_examine = item_to_examine_room;

        // was the item found in the backpack?
        if (item_to_examine_backpack)
            item_to_examine = item_to_examine_backpack;

        // was the item not found?
        if (!item_to_examine)
        {
            printf("I can't see any %s.\n", command->groups[0]);
            return;
        }

        // is the item examinable?
        if (!((item_to_examine->item->properties & 0x0004) == 0x0004))
        {
            printf("There's nothing to say about %s.\n", item_to_examine->item->name);
            return;
        }
        else
        {
            // examine the item
            printf("%s.\n", item_to_examine->item->description);
            return;
        }
    }

    //-------------------------------------------------------------------------INVENTORY----------------------------

    if (strcmp(command->name, "Inventory") == 0)
    {
        // is the backpack empty?
        if (game->backpack->size == 0)
            printf("There are no items in your backpack.\n");

        // display the items in your backpack
        if (game->backpack->size > 0)
        {
            printf("These items are in your backpack:\n");
            for (struct container *head = game->backpack->items; head; head = head->next)
                printf("%s\n", head->item->name);
        }
    }

    //-------------------------------------------------------------------------RESTART------------------------------

    if (strcmp(command->name, "Restart") == 0)
    {
        char input[4];

        printf("Are you sure you want to restart the game? All progress will be lost.\n");

        fgets(input, 4, stdin);
        input[strlen(input) - 1] = '\0';

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
        {
            printf("I didn't understand that.\n");
            return;
        }

        if (!yes)
            game->state = RESTART;

        if (!no)
        {
            printf("Okay.\n");
            return;
        }
    }

    //-------------------------------------------------------------------------QUIT---------------------------------

    if (strcmp(command->name, "Quit") == 0)
    {
        char input[20];

        printf("Are you sure you want to quit the game? All progress will be lost.\n");

        scanf("%s", input);

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
        {
            printf("Goodbye.\n");
            game->state = GAMEOVER;
        }

        if (!no)
            printf("Okay.\n");
    }

    //-------------------------------------------------------------------------SAVE---------------------------------

    if (strcmp(command->name, "Save") == 0)
    {
        FILE *f = fopen("saves/savegame.txt", "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        //########################################################################

        struct container *cmd_current = game->parser->history;
        struct container *cmd_hold_next = NULL;

        int commands_in_history = 0;

        for (; cmd_current; cmd_current = cmd_current->next)
            commands_in_history++;

        if (commands_in_history == 0)
        {
            printf("Nothing to save\n");
            return;
        }

        for (int i = commands_in_history; i > 0; --i)
        {
            struct container *head = game->parser->history;

            for (int index = 1; head != NULL; head = head->next, index++)
            {
                if (index == i)
                {
                    printf("Writing %s\n", head->text);
                    fprintf(f, "%s", head->text);
                    fprintf(f, "\n");
                }
            }
        }

        fclose(f);
        f = NULL;

        printf("Game saved.\n");
        return;
    }

    //-------------------------------------------------------------------------LOAD---------------------------------

    if (strcmp(command->name, "Load") == 0)
    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        FILE *f = fopen("saves/savegame.txt", "r");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        fseek(f, 0, SEEK_SET);

        game->backpack = destroy_backpack(game->backpack);
        game->backpack = create_backpack(BACKPACK_CAPACITY);
        game->world = destroy_world(game->world);
        game->world = create_world();
        game->current_room = game->world->room;
        game->state = PLAYING;

        for (struct container *head = game->parser->history; head; head = head->next)
        {
            printf("History: %s\n", head->text);
        }

        game->parser = destroy_parser(game->parser);
        game->parser = create_parser();

        while ((read = getline(&line, &len, f)) != -1)
        {
            line[strlen(line) - 1] = '\0';
            printf("%s\n", line);

            struct command *cmd = parse_input(game->parser, line);
            if (cmd)
            {
                printf("Command to execute: %s\n", cmd->name);
                execute_command(game, cmd);
            }
        }

        for (struct container *head = game->parser->history; head; head = head->next)
        {
            printf("History: %s\n", head->text);
        }

        fclose(f);

        printf("Game loaded?\n");
        return;
    }

    //-------------------------------------------------------------------------VERSION------------------------------

    if (strcmp(command->name, "Version") == 0)
    {
        printf("This is the best version. by Matej and Veronika.\n");
    }

    //-------------------------------------------------------------------------ABOUT--------------------------------

    if (strcmp(command->name, "About") == 0)
    {
        printf("This game is very nice.\n");
    }

    //-------------------------------------------------------------------------HELP---------------------------------

    if (strcmp(command->name, "Help") == 0)
    {
        printf("I don't want to help you. Sorry.\n");
    }
}
