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
}

struct container *destroy_world(struct container *world)
{
}

struct room *get_room(struct container *world, char *name)
{
}