#include "backpack.h"
#include <stdlib.h>


BACKPACK *create_backpack(const int capacity) {
    // alloc for backpack struct
    BACKPACK *backpack = malloc(sizeof(BACKPACK));

    // set backpack capacity
    backpack->capacity = capacity;

    return backpack;
}

BACKPACK *destroy_backpack(BACKPACK *backpack) {

    for (int i=0; i<backpack->size; i++){
        free(&backpack->items[i]);
    }

    // free backpack
    free(backpack);

    return NULL;
}

bool add_item_to_backpack(BACKPACK *backpack, ITEM *item) {
        if (backpack->size < backpack->capacity) {
            backpack->items = item;
            backpack->size++;
            return true;
        } else {
            return false;
        }
}

void delete_item_from_backpack(BACKPACK *backpack, ITEM *item) {

}