#include "container.h"
#include "world.h"

struct container *create_world()
{

    struct room *room_A = create_room("Room A", "This is room A.");
    struct room *room_B = create_room("Room B", "This is room B.");
    struct room *room_C = create_room("Room C", "This is room C.");
    struct room *room_D = create_room("Room D", "This is room D.");
    struct room *room_E = create_room("Room E", "This is room E.");

    struct room *room_F = create_room("Room F", "This is room F.");
    struct room *room_G = create_room("Room G", "This is room G.");
    struct room *room_H = create_room("Room H", "This is room H.");
    struct room *room_I = create_room("Room I", "This is room I.");
    struct room *room_J = create_room("Room J", "This is room J.");

    struct room *room_K = create_room("Room K", "This is room K.");
    struct room *room_L = create_room("Room L", "This is room L.");
    struct room *room_M = create_room("Room M", "This is room M.");
    struct room *room_N = create_room("Room N", "This is room N.");
    struct room *room_O = create_room("Room O", "This is room O.");

    set_exits_from_room(room_A, room_B, NULL, NULL, NULL);
    set_exits_from_room(room_B, room_C, room_A, NULL, NULL);    
    set_exits_from_room(room_C, room_D, room_B, NULL, NULL);
    set_exits_from_room(room_D, room_E, room_C, NULL, NULL);
    set_exits_from_room(room_E, room_F, room_D, NULL, NULL);

    set_exits_from_room(room_F, room_G, room_E, NULL, NULL);
    set_exits_from_room(room_G, room_H, room_F, NULL, NULL);
    set_exits_from_room(room_H, room_I, room_G, NULL, NULL);
    set_exits_from_room(room_I, room_J, room_H, NULL, NULL);
    set_exits_from_room(room_J, room_K, room_I, NULL, NULL);

    set_exits_from_room(room_K, room_L, room_J, NULL, NULL);
    set_exits_from_room(room_L, room_M, room_K, NULL, NULL);
    set_exits_from_room(room_M, room_N, room_L, NULL, NULL);
    set_exits_from_room(room_N, room_O, room_M, NULL, NULL);
    set_exits_from_room(room_O, NULL,   room_N, NULL, NULL);

    ITEM *wooden_sword = create_item("Wooden Sword", "This wooden sword isn't particularly useful.", MOVABLE | EXAMINABLE);
    ITEM *golden_watch = create_item("Golden Watch", "This fancy watch sure adds the bling", MOVABLE | USABLE | EXAMINABLE);
    ITEM *dusty_bed    = create_item("Dusty Bed", "This bed will give you rest and allergies", USABLE | EXAMINABLE);
    ITEM *the_orb      = create_item("The Orb", "A nice orb. You should probably take it to the last room.", MOVABLE | USABLE | EXAMINABLE);
    ITEM *generic_item = create_item("Generic item", "This item is so generic, you don't even want to look at it.", EXAMINABLE);
    ITEM *iron_sword   = create_item("Iron Sword", "This sword is seemingly better than a wooden sword. But do you really need it?", MOVABLE | EXAMINABLE);

    add_item_to_room(room_A, generic_item);
    add_item_to_room(room_A, generic_item);
    add_item_to_room(room_A, dusty_bed);
    add_item_to_room(room_C, wooden_sword);
    add_item_to_room(room_D, generic_item);
    add_item_to_room(room_E, generic_item);
    add_item_to_room(room_H, golden_watch);
    add_item_to_room(room_J, the_orb);
    add_item_to_room(room_L, iron_sword);
    add_item_to_room(room_N, generic_item);

    struct container *world = create_container(NULL, TYPE_ROOM, room_A);

    world = add_room_to_world(world, room_B);
    world = add_room_to_world(world, room_C);
    world = add_room_to_world(world, room_D);
    world = add_room_to_world(world, room_E);

    world = add_room_to_world(world, room_F);
    world = add_room_to_world(world, room_G);
    world = add_room_to_world(world, room_H);
    world = add_room_to_world(world, room_I);
    world = add_room_to_world(world, room_J);

    world = add_room_to_world(world, room_K);
    world = add_room_to_world(world, room_L);
    world = add_room_to_world(world, room_M);
    world = add_room_to_world(world, room_N);
    world = add_room_to_world(world, room_O);

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

    return NULL;
}