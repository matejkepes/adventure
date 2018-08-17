#include "container.h"

struct container *create_world()
{
    struct container *world = create_container(world->room, TYPE_ROOM, NULL);
    world->item = NULL;
    world->room = NULL;
    world->command = NULL;
    world->text = NULL;
}

struct container *add_room_to_world(struct container *world, struct room *room)
{
    if (world == NULL)
    {
        return NULL;
    }

    if (room == NULL)
    {
        return world;
    }

    if (!get_from_container_by_name(world, room->name))
    {
        world = create_container(world, TYPE_ROOM, room);
    }

    return world;
}

struct container *destroy_world(struct container *world)
{
}

struct room *get_room(struct container *world, char *name)
{
}