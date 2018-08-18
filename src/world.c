#include "container.h"

struct container *create_world()
{
    struct container *world = create_container(NULL, TYPE_ROOM, NULL);
    return world;
}

struct container *add_room_to_world(struct container *world, struct room *room)
{
    if (world == NULL)
        return NULL;

    if (room == NULL)
        return world;

    if (!get_from_container_by_name(world, room->name))
        world = create_container(world, TYPE_ROOM, room);

    return world;
}

struct container *destroy_world(struct container *world)
{
    world = destroy_containers(world);
    return NULL;
}

struct room *get_room(struct container *world, char *name)
{
    if (world == NULL)
        return NULL;

    if (name == NULL)
        return NULL;

    if (get_from_container_by_name(world, name))
    {
        struct container *room_to_get = get_from_container_by_name(world, name);
        return room_to_get->room;
    }
}