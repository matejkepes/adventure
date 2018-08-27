#include "container.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
           // printf("Destroying %s\n", current->command->name);
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

int strcmp_lowercase(const char* s1, const char* s2)
{
    while(*s1 && (tolower(*s1) == tolower(*s2)))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void *get_from_container_by_name(struct container *first, const char *name)
{
    struct container *cursor = first;

    while(cursor != NULL){

        if (cursor->type == TYPE_ITEM)
        {
            if(strcmp_lowercase(cursor->item->name, name) == 0)
                return cursor;
        }

        if (cursor->type == TYPE_COMMAND)
        {
            if(strcmp_lowercase(cursor->command->name, name) == 0)
                return cursor;
        }

        if (cursor->type == TYPE_ROOM)
        {
            if(strcmp_lowercase(cursor->room->name, name) == 0)
                return cursor;
        }

        if (cursor->type == TYPE_TEXT)
        {
            if(strcmp_lowercase(cursor->text, name) == 0)
                return cursor;
        }
        
        cursor = cursor->next;
    }
        return NULL;
}

struct container *remove_container(struct container *first, void *entry)
{
    struct container *cursor = first;
    struct container * previous = NULL;

    if (entry == NULL) {
        return first;
    }

    while(cursor != NULL) {

        if (cursor->type == TYPE_ITEM)
        {
            if (cursor->item == entry) {
                if (cursor == first) {
                    struct container *tmp = first->next;
                    first->next = NULL;
                    return tmp;
                } else {
                    //Middle or last

                    previous->next = cursor->next;
                    cursor->next = NULL;

                    return first;
                }
            }  
        }

        if (cursor->type == TYPE_COMMAND)
        {
            if (cursor->command == entry) {
                if (cursor == first) {
                    struct container *tmp = first->next;
                    first->next = NULL;
                    return tmp;
                } else {
                    //Middle or last

                    previous->next = cursor->next;
                    cursor->next = NULL;

                    return first;
                }
            } 
        }

        if (cursor->type == TYPE_ROOM)
        {
            if (cursor->room == entry) {
                if (cursor == first) {
                    struct container *tmp = first->next;
                    first->next = NULL;
                    return tmp;
                } else {
                    //Middle or last

                    previous->next = cursor->next;
                    cursor->next = NULL;

                    return first;
                }
            } 
        }

        if (cursor->type == TYPE_TEXT)
        {
            if (cursor->text == entry) {
                if (cursor == first) {
                    struct container *tmp = first->next;
                    first->next = NULL;
                    return tmp;
                } else {
                    //Middle or last

                    previous->next = cursor->next;
                    cursor->next = NULL;

                    return first;
                }
            } 
        }

        previous = cursor;
        cursor = cursor->next;
    }

    return first;
}
