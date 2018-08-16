#include "backpack.h"
#include <stdlib.h>

BACKPACK *create_backpack(const int capacity)
{

    BACKPACK *backpack = NULL;

    backpack = malloc(sizeof(BACKPACK));

    backpack->items = NULL;
    backpack->capacity = capacity;
    backpack->size = 0;

    return backpack;
}

BACKPACK *destroy_backpack(BACKPACK *backpack)
{

    if (backpack == NULL)
    {
        return NULL;
    }

    destroy_containers(backpack->items);

    // free backpack
    free(backpack);

    return NULL;
}

bool add_item_to_backpack(BACKPACK *backpack, ITEM *item)
{

    if (backpack->capacity > 0)
    {
        if (backpack->size < backpack->capacity)
        {
            backpack->items = create_container(backpack->items, TYPE_ITEM, item);
            backpack->size++;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void delete_item_from_backpack(BACKPACK *backpack, ITEM *item)
{
    if (backpack != NULL && item != NULL)

    {
        struct container *tmp = get_from_container_by_name(backpack->items, item->name);

        if (tmp != NULL)
        {
            backpack->items = remove_container(backpack->items, tmp->item);
            backpack->size--;
        }
    }
}

ITEM *get_item_from_backpack(const BACKPACK *backpack, char *name)
{
    if (backpack != NULL && name != NULL)
    {
        struct container *tmp = get_from_container_by_name(backpack->items, name);

        return tmp == NULL ? NULL : tmp->item;
    }

    return NULL;
}