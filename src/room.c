#include "room.h"
#include <string.h>
#include <stdio.h>

struct room *create_room(char *name, char *description)
{
    if (name != NULL && description != NULL)
    {
        struct room *room = malloc(sizeof(struct room));
        room->name = strdup(name);
        room->description = strdup(description);
        room->north = NULL;
        room->south = NULL;
        room->east = NULL;
        room->west = NULL;
        room->items = NULL;
        return room;
    }

    return NULL;
}

struct room *destroy_room(struct room *room)
{
    if (room != NULL)
    {

        free(room->name);
        room->name = NULL;
        free(room->description);
        room->description = NULL;
        room->north = NULL;
        room->south = NULL;
        room->east = NULL;
        room->west = NULL;
        destroy_containers(room->items);
        room->items = NULL;

        free(room);
    }

    return NULL;
}

void set_exits_from_room(struct room *room, struct room *north, struct room *south, struct room *east, struct room *west)
{
    if (room != NULL)
    {
        room->north = north;
        room->south = south;
        room->east = east;
        room->west = west;
    }
}

void show_room(const struct room *room)
{
    if (room != NULL)
    {
        printf("%s\n", room->name);
        printf("%s\n", room->description);
        printf("You see ");

        for (struct container *head = room->items; head != NULL; head = head->next)
        {
            printf("%s, ", head->item->name);
        }

        printf("\n");
    }
}

void delete_item_from_room(struct room *room, ITEM *item)
{
    if (room != NULL && item != NULL)
    {
        struct container *item_to_delete = get_from_container_by_name(room->items, item->name);
        //       printf("item to delete: %d\n", item_to_delete == NULL);
        room->items = remove_container(room->items, item_to_delete->item);
        //       printf("Heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeere \n");
    }
}

void add_item_to_room(struct room *room, ITEM *item)
{
    if (room != NULL && item != NULL)
    {
        room->items = create_container(room->items, TYPE_ITEM, item);
    }
}

ITEM *get_item_from_room(const struct room *room, const char *name)
{
    if (room != NULL && name != NULL)
    {
        struct container *result = get_from_container_by_name(room->items, name);

        if (result != NULL)
            return result->item;
    }

    return NULL;
}