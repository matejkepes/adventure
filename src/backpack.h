#ifndef BACKPACK_H
#define BACKPACK_H

#include <stdbool.h>

#include "item.h"
#include "container.h"

/**
 * Structure representing the backpack
 */
typedef struct BACKPACK_STRUCT {
    int capacity;               /// backpack capacity (max nr. of items)
    int size;                   /// current size
    struct container* items;    /// list of items
} BACKPACK;


/**
 * Creates backpack
 * @param capacity backpack capacity
 * @return Reference to created backpack.
 */
BACKPACK *create_backpack(const int capacity);


/**
 * Destroys backpack
 * @param backpack backpack to destroy
 * @return Always returns NULL.
 */
BACKPACK *destroy_backpack(BACKPACK *backpack);


/**
 * Adds item to backpack
 * @param backpack
 * @param item
 * @return true, if item was added successfully, false otherwise (backpack is full)
 */
bool add_item_to_backpack(BACKPACK *backpack, ITEM *item);


/**
 * Deletes given item from backpack
 * @param backpack backpack, from which the item will be deleted
 * @param item item to delete
 */
void delete_item_from_backpack(BACKPACK *backpack, ITEM *item);


/**
 * Gets the item by name
 * @param backpack backpack to search for item
 * @param name name of item to get
 * @return Reference to the item, if found, NULL otherwise.
 */
ITEM *get_item_from_backpack(const BACKPACK *backpack, char* name);

#endif
