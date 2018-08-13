#include "item.h"
#include <string.h>

ITEM *create_item (char *name, char *description, unsigned int properties) {
    // alloc for item struct
    ITEM *item = malloc(sizeof(ITEM));

    // alloc for item->name
    item->name = strdup(name);

    // alloc for item->description
    item->description = strdup(description);

    return item;
}

ITEM *destroy_item(ITEM *item) {
    // free item->name
    free(item->name);
    item->name = NULL;

    // free item->description
    free(item->description);
    item->description = NULL;

    return NULL;
}