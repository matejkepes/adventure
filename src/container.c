#include "container.h"
#include <stdlib.h>
#include <string.h>

struct container *create_container(struct container *first, enum container_type type, void *entry)
{
    struct container *container = malloc(sizeof(struct container));
    container->next = first;
    container->type = type;

    if (container->type == TYPE_ROOM)
    {
        container->room = (struct room *)entry;
    }

    if (container->type == TYPE_ITEM)
    {
        container->item = (ITEM *)entry;
    }

    if (container->type == TYPE_COMMAND)
    {
        container->command = (COMMAND *)entry;
    }

    if (container->type == TYPE_TEXT)
    {
        container->text = (char *)entry;
    }

    return container;
}

struct container *destroy_containers(struct container *first)
{
    if (first == NULL)
        return NULL;

    struct container *current = first;

    while (current != NULL)
    {
        struct container *next = current->next;

        if (current->type == TYPE_ITEM)
        {
            destroy_item(current->item);
            current->item = NULL;
        }

        if (current->type == TYPE_COMMAND)
        {
            destroy_command(current->command);
            current->command = NULL;
        }

        if (current->type == TYPE_ROOM)
        {
            destroy_room(current->room);
            current->room = NULL;
        }

        if (current->type == TYPE_TEXT)
        {
            free(current->text);
        }

        current = next;
    }

    return NULL;
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
void *get_from_container_by_name(struct container *first, const char *name)
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
struct container *remove_container(struct container *first, void *entry)
{
}
