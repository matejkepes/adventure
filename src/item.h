#ifndef ITEM_H
#define ITEM_H

#include <stdlib.h>
/**
 * Item properties
 */
enum properties{
    NONE        = 0,    // no extra properties
    MOVABLE     = 1,    // item can be carried in the backpack
    USABLE      = 2,    // item can be used
    EXAMINABLE  = 4,    // item can be examined
    OPENABLE    = 8     // item can be opened or closed
};

/**
 * Game item
 */
typedef struct ITEM_STRUCT {
    char*         name;         /// item name
    char*         description;  /// item description
    unsigned int  properties;   /// item properties
} ITEM;


/**
 * Creates new item
 *
 * Function creates new item in the memory and returns it's reference. Every item
 * has name, description and set of properties. If name or description was not
 * given, the item can't be created and function returns NULL.
 * @param name item name
 * @param description item description
 * @param properties item properties
 * @return Reference to the newly created item or NULL, if item could not be created.
 */
ITEM * create_item(char* name, char* description, unsigned int properties);


/**
 * Destroys item
 *
 * Frees all the memory resources which were used with the item. Function a
 * always returns NULL as the new object reference.
 * @param item item to destroy
 * @return Always returns NULL.
 */
ITEM *destroy_item(ITEM *item);

#endif
