#include "container.h"
#include <stdlib.h>
#include <string.h>

struct container* create_container(struct container* first, enum container_type type, void* entry) 
{
    struct container *container = malloc(sizeof(container));

    container->next = NULL;
    container->type = type;
    
    if (container->type == TYPE_ROOM) {
        container->room = strdup(entry);
    } 

    if (container->type == TYPE_ITEM) {
        container->item = strdup(entry);
    }

    if (container->type == TYPE_COMMAND) {
        container->command = strdup(entry);
    }

    if (container->type == TYPE_TEXT) {
        container->text = strdup(entry);
    }
    
    return first;
}

struct container* destroy_containers(struct container* first) 
{

}


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
void* get_from_container_by_name(struct container *first, const char *name) 
{

}


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
struct container* remove_container(struct container *first, void *entry)
{
    
}
