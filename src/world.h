#ifndef WORLD_H
#define WORLD_H

#include "room.h"


/**
 * Creates the game world
 *
 * This function is responsible for the creation of virtual world.
 * It creates the complete list of the game locations (rooms) and
 * their interconnections through their exits.
 * Finally returns reference to the created world.
 * @return Reference to the list of all available game rooms
 */
struct container* create_world();


/**
 * Adds new room to the world
 *
 * Function adds new room to the given world and returns reference
 * to this world. The room can be added only if it's name is unique in whole
 * world. If there is already a room with such name, room is not added to the
 * world.
 * @param world reference to the list of existing rooms
 * @param room room to add
 * @return Reference to the world.
 */
struct container* add_room_to_world(struct container* world, struct room* room);


/**
 * Destroys all existing rooms
 *
 * Function destroys the list of existing rooms in the world. It
 * destroys both the container and the room that was referenced
 * from the container.
 * Function returns NULL as the new reference for the world.
 * @param world reference to the list of existing rooms
 * @return Always returns NULL
 */
struct container* destroy_world(struct container* world);


/**
 * Returns room by it's name
 *
 * Function returns reference to the room from given world based
 * on it's name. If there is no such room in the world, NULL is
 * returned.
 * @param world reference to the list of existing rooms
 * @param name the name of the room to get
 * @return Reference to the room or NULL, if room was not found.
 */
struct room* get_room(struct container* world, char* name);

#endif
