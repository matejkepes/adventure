#include "room.h"

struct room* create_room(char *name, char *description) {

}

struct room* destroy_room(struct room* room) {

}

void set_exits_from_room(struct room *room, struct room *north, struct room *south, struct room *east, struct room *west){

}


void show_room(const struct room* room){

}


void delete_item_from_room(struct room* room, ITEM * item){

}

void add_item_to_room(struct room* room, ITEM * item){
    
}

ITEM * get_item_from_room(const struct room* room, const char* name){

}