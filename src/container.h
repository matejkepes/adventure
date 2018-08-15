#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdbool.h>

#include "room.h"
#include "item.h"
#include "command.h"

/**
 * Container type
 *
 * Enumeration type defining the type of container data. It can be one of four
 * types: ROOM, ITEM, COMMAND or TEXT.
 */
enum container_type {
    TYPE_ROOM, TYPE_ITEM, TYPE_COMMAND, TYPE_TEXT
};


/**
 * Container structure
 *
 * Every list used in the game is of type of this structure. The union inside
 * represents the container data, which is pointer to room, item, command
 * or text.
 */
struct container {
    enum container_type type;       /// type of container
    union{
        struct room*    room;       /// pointer to room
        ITEM *    item;       /// pointer to item
        COMMAND* command;    /// pointer to command
        char*           text;       /// pointer to text
    };
    struct container* next;         /// pointer to next data container in list
};


/**
 * Creates and appends new container to list
 *
 * Creates new container, which will hold reference to room or item or command
 * or text. Function returns reference to the first container of this list.
 * @param first reference to the first container of list
 * @param type type of container
 * @param entry container data
 * @return Reference to the first container of the list, or NULL, if container could not be created.
 */
struct container* create_container(struct container* first, enum container_type type, void* entry);


/**
 * Destroys all containers in the list
 *
 * This function destroys all the containers in the list, which starts with the
 * container specified with the parameter first. Function returns value NULL as
 * the new reference to this list of containers.
 * @param first pointer to the first container of the list
 * @return Function always returns NULL as the pointer to this list.
 */
struct container* destroy_containers(struct container* first);


/**
 * Returns container content based on it's name
 *
 * Function returns container content of specific type based on it's name. So it
 * returns command object based on the command name, or item object based on the
 * item name, or room object based on the room name, or text entry if such entry
 * exists. Search pattern is not case sensitive. If such object doesn't exist,
 * NULL is returned.
 * @param first pointer to the first container of the list
 * @param name search pattern
 * @return Reference to the founded object or NULL, if the object was not found.
 */
void* get_from_container_by_name(struct container *first, const char *name);


/**
 * Deletes container, which holds given entry leaving the entry intact
 *
 * This function destroys only one container in the list. The container is
 * identified based on the pointer to the entry it contains. Content of the
 * container stays unchanged! Function returns reference to the beginning of
 * this list.
 * @param first pointer to the first container of the list
 * @param entry container entry
 * @return Reference to the list of containers without given container.
 */
struct container* remove_container(struct container *first, void *entry);

#endif
