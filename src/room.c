#include "room.h"

struct room *create_room(char *name, char *description)
{
    if (name != NULL && description != NULL)
    {
        struct room *room = malloc(sizeof(struct room));
        room->name = name;
        room->description = description;
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
    if (room != NULL) {
       
        room->name = NULL;
        room->description = NULL;
        room->north = NULL;
        room->south = NULL;
        room->east = NULL;
        room->west = NULL;
        destroy_containers(room->items);
        room->items = NULL;

        free(room);

        return NULL;
    }

    return NULL;
}

void set_exits_from_room(struct room *room, struct room *north, struct room *south, struct room *east, struct room *west)
{
}

void show_room(const struct room *room)
{
}

void delete_item_from_room(struct room *room, ITEM *item)
{
}

void add_item_to_room(struct room *room, ITEM *item)
{
}

ITEM *get_item_from_room(const struct room *room, const char *name)
{
}