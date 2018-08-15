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

void *get_from_container_by_name(struct container *first, const char *name)
{
    struct container *cursor = first;

    while(cursor != NULL){

        if (cursor->type == TYPE_ITEM)
        {
            if(strcmp(cursor->item->name, name) == 0)
                return cursor;
        }

        if (cursor->type == TYPE_COMMAND)
        {
            if(strcmp(cursor->command->name, name) == 0)
                return cursor;
        }

        if (cursor->type == TYPE_ROOM)
        {
            if(strcmp(cursor->room->name, name) == 0)
                return cursor;
        }

        if (cursor->type == TYPE_TEXT)
        {
            if(strcmp(cursor->text, name) == 0)
                return cursor;
        }
        
        cursor = cursor->next;
    }
        return NULL;
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
