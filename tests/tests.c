#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "item.h"
#include "container.h"
#include "backpack.h"
#include "room.h"
#include "world.h"
#include "parser.h"

static void createCommand_noMatches_returnsCommandsWithGroupsEqualsNull(void **state)
{
    COMMAND *command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t)0);

    assert_true(command->nmatch == (size_t)0);
    assert_string_equal(command->name, "TEST_COMMAND");
    assert_string_equal(command->description, "TEST_COMMAND_DESC");
    assert_true(command->groups == NULL);
}

static void createCommand_withMatches_returnsCommandsWithGroupsNotNull(void **state)
{
    COMMAND *command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t)5);

    assert_true(command->nmatch == (size_t)5);
    assert_string_equal(command->name, "TEST_COMMAND");
    assert_string_equal(command->description, "TEST_COMMAND_DESC");
    assert_true(command->groups != NULL);
}

static void test_destroy_command(void **state)
{
    //SETUP
    COMMAND *command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t)0);

    //TEST
    destroy_command(command);

    //Assertion
    assert_true(command->name == NULL);
    assert_true(command->description == NULL);
}

static void createItem_allocatedProperly(void **state)
{
    ITEM *item = create_item("SWORD", "MIGHTY SWORD", 0x0000);

    assert_string_equal(item->name, "SWORD");
    assert_string_equal(item->description, "MIGHTY SWORD");
    assert_true(item->properties == 0x0000);
}

static void createContainer_firstItemNull_returnsNewContainer(void **state)
{
    ITEM *item = create_item("SWORD", "MIGHTY SWORD", 0x0000);
    struct container *container = NULL;

    container = create_container(NULL, TYPE_ITEM, item);

    //  State we're testing for
    //  ############
    //  # ITEM_ONE # ---> NULL
    //  ############

    assert_true(container != NULL);
    assert_true(container->next == NULL);
    assert_true(container->type == TYPE_ITEM);
    assert_ptr_equal(container->item, item);
}

static void createContainer_firstItemNotNull_returnsNewContainerItemIsNewHead(void **state)
{
    //Setup first container item
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    struct container *first = (struct container *)malloc(sizeof(struct container));
    first->item = itemOne;
    first->next = NULL;

    //  After setup
    //  ############
    //  # ITEM_ONE # ---> NULL
    //  ############

    struct container *container = NULL;
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);

    container = create_container(first, TYPE_ITEM, itemTwo);

    //  This is the state we're expecting
    //  ############        ############
    //  # ITEM_TWO #  ----> # ITEM_ONE # ---> NULL
    //  ############        ############

    assert_true(container != NULL);
    //The head of linked list must be the last item inserted
    assert_ptr_equal(container->item, itemTwo);
    //The next item must be the first inserted item
    assert_ptr_equal(container->next->item, itemOne);
    //Assert that we only have 2 items
    assert_true(container->next->next == NULL);
}

static void destroyContainers_NULLAsFirst_returnsNull(void **state)
{
    struct container *destroyed = destroy_containers(NULL);

    assert_null(destroyed);
}

static void destroyContainers_FirstItemPassedIn_returnsNullOBjectsFreed(void **state)
{
    //SETUP
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);

    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);

    //TEST
    struct container *destroyed = destroy_containers(third);

    //ASSERT
    assert_null(destroyed);
    assert_null(first->item);
    assert_null(second->item);
    assert_null(third->item);
    assert_null(itemOne->name);
    assert_null(itemTwo->name);
    assert_null(itemThree->name);
}

static void getFromContainer_ListNullReturnsNull(void **state)
{
    assert_null(get_from_container_by_name(NULL, "WHATEVER"));
}

static void getFromContainer_ListContainsItem_ItemRetrieved(void **state)
{
    //SETUP
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);

    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);

    assert_null(get_from_container_by_name(third, "ITEM_FOUR"));
    assert_ptr_equal(get_from_container_by_name(third, "ITEM_oNE"), first);
    assert_ptr_equal(get_from_container_by_name(third, "ITEM_TwO"), second);
    assert_ptr_equal(get_from_container_by_name(third, "ITEM_ThREE"), third);
}

int get_linked_list_size(struct container *container)
{
    if (container == NULL)
        return 0;

    int size;
    struct container *head = container;

    for (size = 0; head != NULL; head = head->next, size++)
        ;

    return size;
}

struct container *get_nth_item_from_linked_list(struct container *container, int n)
{
    if (container == NULL || n < 0)
        return NULL;

    int index;
    struct container *head = container;

    for (index = 0; head != NULL; head = head->next, index++)
    {
        if (index == n)
            return head;
    }

    return NULL;
}

static void removeContainer__ListNullEntryNull__returnsNull(void **state)
{
    assert_null(remove_container(NULL, NULL));
}

static void removeContainer__ListNullEntryNotNull__returnsNull(void **state)
{
    assert_null(remove_container(NULL, create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000)));
}

static void removeContainer__EntryNull__returnsUnchagedList(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    assert_ptr_equal(first, remove_container(first, NULL));
}

static void removeContainer__EntryNotInList_returnsReferenceListUnchanged(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);

    assert_ptr_equal(first, remove_container(first, itemTwo));
}

static void removeContainer__EntryInList__returnsReferenceListChanged(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);

    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);

    struct container *afterFirstItemRemoved = remove_container(third, itemOne);
    assert_int_equal(2, get_linked_list_size(afterFirstItemRemoved));

    struct container *afterSecondItemRemoved = remove_container(afterFirstItemRemoved, itemTwo);
    assert_int_equal(1, get_linked_list_size(afterSecondItemRemoved));

    struct container *afterThirdItemRemoved = remove_container(afterSecondItemRemoved, itemThree);
    assert_int_equal(0, get_linked_list_size(afterThirdItemRemoved));
}

static void removeContainer__EntryInListListOfSizeOne_returnsNull(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    assert_null(remove_container(first, itemOne));
}

static void removeContainer_EntryInMiddle_ReturnsReferenceListChanged(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);

    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);

    struct container *afterItemInTheMiddleRemoved = remove_container(third, itemTwo);

    assert_int_equal(2, get_linked_list_size(afterItemInTheMiddleRemoved));
    assert_null(second->next);
    assert_ptr_equal(third, get_nth_item_from_linked_list(afterItemInTheMiddleRemoved, 0));
    assert_ptr_equal(first, get_nth_item_from_linked_list(afterItemInTheMiddleRemoved, 1));
}

static void removeFromContainer_ListNotEmpty_ItemRemovedProperly(void **state)
{
    //SETUP
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);

    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);

    struct container *current_head = remove_container(second, itemTwo);
    assert_non_null(current_head);
    assert_ptr_equal(first, current_head);
    assert_null(remove_container(current_head, itemOne));
}

static void backpack_create_backpack(void **state)
{
    const int BACKPACK_CAPACITY = 10;
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);

    assert_non_null(backpack);
    assert_int_equal(0, backpack->size);
    assert_int_equal(BACKPACK_CAPACITY, backpack->capacity);
    assert_null(backpack->items);
}

BACKPACK *createBackpackWithItems(int capacity, struct container *items)
{
    BACKPACK *backpack = malloc(sizeof(BACKPACK));
    backpack->items = items;
    backpack->size = get_linked_list_size(items);
    backpack->capacity = capacity;

    return backpack;
}

static void backpack_destroy_backpack_backpackNullReturnsNull(void **state)
{
    assert_null(destroy_backpack(NULL));
}

static void backpack_destroy_backpack(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);
    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);

    BACKPACK *backpack = createBackpackWithItems(10, third);

    //Essentially !!! destroy_containers must be called
    assert_null(destroy_backpack(backpack));
    assert_null(itemOne->name);
    assert_null(itemTwo->name);
    assert_null(itemThree->name);
    assert_null(first->item);
    assert_null(second->item);
    assert_null(third->item);
}

static void backpack_add_item_to_backpack_returnsFalseWhenCapacity0(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    BACKPACK *backpack = createBackpackWithItems(0, NULL);

    assert_false(add_item_to_backpack(backpack, itemOne));
    assert_null(backpack->items);
    assert_int_equal(0, backpack->size);
}

static void backpack_add_item_to_backpack_returnsTrueWhenCapacityIsNotExceeded(void **state)
{
    BACKPACK *backpack = createBackpackWithItems(1, NULL);
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);

    assert_true(add_item_to_backpack(backpack, itemOne));
    assert_ptr_equal(backpack->items->item, itemOne);
    assert_int_equal(1, backpack->size);
}

static void backpack_add_item_to_backpack_returnsFalseWhenMaxCapacityReached(void **state)
{
    BACKPACK *backpack = createBackpackWithItems(1, NULL);
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);

    assert_true(add_item_to_backpack(backpack, itemOne));
    assert_false(add_item_to_backpack(backpack, itemTwo));
    assert_ptr_equal(backpack->items->item, itemOne);
    assert_int_equal(1, backpack->size);
}

static void backpack_delete_item_from_backpack_backpackNull_DoesntCrash(void **state)
{
    delete_item_from_backpack(NULL, NULL);
}

static void backpack_delete_item_from_backpack_ItemNull_DoesntCrash(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);

    delete_item_from_backpack(backpack, NULL);
}

static void backpack_delete_item_from_backpack_ItemRemovedWhenInBackpack(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    BACKPACK *backpack = createBackpackWithItems(BACKPACK_CAPACITY, create_container(NULL, TYPE_ITEM, itemOne));

    delete_item_from_backpack(backpack, itemOne);
    assert_int_equal(backpack->size, 0);
    assert_int_equal(backpack->capacity, BACKPACK_CAPACITY);
    assert_null(backpack->items);
}

static void backpack_delete_item_from_backpack_ItemNotRemoved_WhenNotInBackpack(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);

    BACKPACK *backpack = createBackpackWithItems(BACKPACK_CAPACITY, create_container(NULL, TYPE_ITEM, itemOne));

    //Delete an item which is not in the backpack
    delete_item_from_backpack(backpack, itemTwo);

    assert_int_equal(backpack->size, 1);
    assert_int_equal(backpack->capacity, BACKPACK_CAPACITY);
    assert_ptr_equal(backpack->items->item, itemOne);
}
static void backpack_get_item_backpackOrNameNull_returnsNull(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    BACKPACK *backpack = createBackpackWithItems(10, create_container(NULL, TYPE_ITEM, itemOne));

    assert_null(get_item_from_backpack(NULL, "ITEM"));
    assert_null(get_item_from_backpack(backpack, NULL));
}

static void backpack_get_item_ItemNotInBackpack_returnsNull(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    BACKPACK *backpack = createBackpackWithItems(BACKPACK_CAPACITY, create_container(NULL, TYPE_ITEM, itemOne));

    assert_null(get_item_from_backpack(backpack, "SOME_OTHER_ITEM"));
}

static void backpack_get_item_ItemInBackpack_ItemReturnedBackpackItemsIntact(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    BACKPACK *backpack = createBackpackWithItems(BACKPACK_CAPACITY, create_container(NULL, TYPE_ITEM, itemOne));

    assert_ptr_equal(get_item_from_backpack(backpack, "ITEM_ONE"), itemOne);
}

static void Room_create_room(void **state)
{
    assert_null(create_room(NULL, NULL));
    assert_null(create_room("NAME", NULL));
    assert_null(create_room(NULL, "DESC"));

    struct room *room = create_room("NAME", "DESC");
    assert_string_equal(room->name, "NAME");
    assert_string_equal(room->description, "DESC");
}

static void Room_destroy_room(void **state)
{
    char *name = strdup("NAME");
    char *description = strdup("DESC");

    struct room *room = malloc(sizeof(struct room));
    room->name = name;
    room->description = description;

    char **tmp_name = &(room->name);
    char **tmp_desc = &(room->description);

    room = destroy_room(room);
}

static void Room_set_exits_from_room(void **state)
{
    struct room *room = malloc(sizeof(struct room));
    struct room *N = malloc(sizeof(struct room));
    struct room *S = malloc(sizeof(struct room));
    struct room *E = malloc(sizeof(struct room));
    struct room *W = malloc(sizeof(struct room));

    set_exits_from_room(room, N, S, E, W);
    assert_ptr_equal(room->north, N);
    assert_ptr_equal(room->south, S);
    assert_ptr_equal(room->east, E);
    assert_ptr_equal(room->west, W);

    set_exits_from_room(room, NULL, NULL, NULL, NULL);
    assert_null(room->north);
    assert_null(room->south);
    assert_null(room->east);
    assert_null(room->west);
}

static void Room_show(void **state)
{
    //Just to see if it crashes or not
    show_room(NULL);
}

static void Room_delete_item_from_room(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    struct container *head = create_container(NULL, TYPE_ITEM, itemOne);
    head = create_container(head, TYPE_ITEM, itemTwo);

    struct room *room = malloc(sizeof(struct room));
    room->items = head;

    delete_item_from_room(NULL, NULL);
    delete_item_from_room(NULL, itemOne);
    delete_item_from_room(room, NULL);

    assert_ptr_equal(room->items, head);

    delete_item_from_room(room, itemTwo);
    assert_ptr_equal(room->items->item, itemOne);

    delete_item_from_room(room, itemOne);

    assert_null(room->items);
}

static void Room_add_item_to_room(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    struct room *room = malloc(sizeof(struct room));

    assert_int_equal(get_linked_list_size(room->items), 0);

    add_item_to_room(room, itemOne);

    assert_int_equal(get_linked_list_size(room->items), 1);
    assert_ptr_equal(room->items->item, itemOne);

    add_item_to_room(room, itemTwo);

    assert_int_equal(get_linked_list_size(room->items), 2);
    assert_ptr_equal(room->items->item, itemTwo);
}

static void Room_get_item_from_room(void **state)
{
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);

    struct container *head;
    head = create_container(NULL, TYPE_ITEM, itemOne);
    head = create_container(head, TYPE_ITEM, itemTwo);

    struct room *room = malloc(sizeof(struct room));
    room->items = head;

    assert_null(get_item_from_room(NULL, NULL));

    assert_null(get_item_from_room(NULL, "TEST"));

    assert_null(get_item_from_room(room, NULL));

    assert_null(get_item_from_room(room, "ITEM_THREE"));

    //Also checks if item stays in the room.
    assert_ptr_equal(get_item_from_room(room, "ITEM_tWO"), itemTwo);
    assert_ptr_equal(get_item_from_room(room, "ITEM_TwO"), itemTwo);
    assert_ptr_equal(get_item_from_room(room, "ItEM_oNE"), itemOne);
    assert_ptr_equal(get_item_from_room(room, "ITEM_OnE"), itemOne);
}

static void World_create_world(void **state)
{
    struct container *world = create_world();
    assert_null(world->item);
    assert_null(world->room);
    assert_null(world->text);
    assert_null(world->command);

    assert_int_equal(world->type, TYPE_ROOM);
}

static void World_add_room_to_world(void **state)
{
    struct room *roomOne = create_room("ROOM_One", "DESCRIPTION");
    struct room *roomTwo = create_room("ROOM_Two", "DESCRIPTION");
    struct room *roomThree = create_room("ROOM_Three", "DESCRIPTION");
    struct room *roomFour = create_room("ROOM_Four", "DESCRIPTION");

    struct container * world = malloc(sizeof(struct container));
    world->type = TYPE_ROOM;
    world->room = roomFour;

    assert_null(add_room_to_world(NULL, NULL));
    assert_null(add_room_to_world(NULL, roomOne));
    assert_ptr_equal(world, add_room_to_world(world, NULL));

    world = add_room_to_world(world, roomThree);
    world = add_room_to_world(world, roomTwo);
    world = add_room_to_world(world, roomOne);

    assert_ptr_equal(roomOne, get_nth_item_from_linked_list(world, 0)->room);
    assert_ptr_equal(roomTwo, get_nth_item_from_linked_list(world, 1)->room);
    assert_ptr_equal(roomThree, get_nth_item_from_linked_list(world, 2)->room);
    assert_ptr_equal(roomFour, get_nth_item_from_linked_list(world, 3)->room);

     struct room * duplicate = create_room("ROOM_Four", "DESCRIPTION");
     world = add_room_to_world(world, duplicate);

    assert_int_equal(4, get_linked_list_size(world));
}
static void World_destroy_world(void **state)
{
    struct container *world = create_world();
    assert_null(destroy_world(world));
    assert_null(destroy_world(NULL));
}

static void World_get_room(void **state)
{
    struct room *roomOne = create_room("ROOM_One", "DESCRIPTION");
    struct room *roomTwo = create_room("ROOM_Two", "DESCRIPTION");
    struct room *roomThree = create_room("ROOM_Three", "DESCRIPTION");
    struct room *roomFour = create_room("ROOM_Four", "DESCRIPTION");

    struct container * world = malloc(sizeof(struct container));
    world->type = TYPE_ROOM;
    world->room = roomFour;


    world = add_room_to_world(world, roomThree);
    world = add_room_to_world(world, roomTwo);
    world = add_room_to_world(world, roomOne);

    assert_null(get_room(NULL, NULL));
    assert_null(get_room(NULL, "ROOM"));

    assert_null(get_room(world, "ROOM"));

    assert_ptr_equal(roomOne, get_room(world, "RoOm_OnE"));
    assert_ptr_equal(roomTwo, get_room(world, "RoOm_TWO"));
    assert_ptr_equal(roomThree, get_room(world, "RoOm_ThreE"));
    assert_ptr_equal(roomFour, get_room(world, "RoOm_FoUr"));

    assert_ptr_equal(roomOne, get_room(world, "RoOm_OnE"));
    assert_ptr_equal(roomTwo, get_room(world, "RoOm_TWO"));
    assert_ptr_equal(roomThree, get_room(world, "RoOm_ThreE"));
    assert_ptr_equal(roomFour, get_room(world, "RoOm_FoUr"));
}

static void Parser_create_parser(void **state)
{
    assert_non_null(create_parser());
}

static void Parser_destroy_parser(void **state)
{
    assert_null(destroy_parser(create_parser()));
}

static void Parser_parse_input(void **state)
{
    struct parser * parser = malloc(sizeof(struct parser));
    COMMAND * start = create_command("START", "STARTS SOMETHING", "^\\s+START\\s+$", 0);
    COMMAND * examine = create_command("EXAMINE", "Examine an item SOMETHING", "^\\s+EXAMINE\\+(.*)\\+\\s+$", 0);
    assert_non_null(start);


    parser->commands = create_container(create_container(NULL, TYPE_COMMAND, start), TYPE_COMMAND, examine);

    assert_ptr_equal(parse_input(parser, "stArT"), start);
    assert_ptr_equal(parse_input(parser, " eXaMinE iphone"), examine);
    assert_string_equal(examine->groups[0], "iphone");
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        //Command - Create
        cmocka_unit_test(createCommand_noMatches_returnsCommandsWithGroupsEqualsNull),
        cmocka_unit_test(createCommand_withMatches_returnsCommandsWithGroupsNotNull),
        //Command - Destroy
        cmocka_unit_test(test_destroy_command),
        //Item - Create
        cmocka_unit_test(createItem_allocatedProperly),
        //Container - Create
        cmocka_unit_test(createContainer_firstItemNull_returnsNewContainer),
        cmocka_unit_test(createContainer_firstItemNotNull_returnsNewContainerItemIsNewHead),
        //Container - Destroy
        cmocka_unit_test(destroyContainers_NULLAsFirst_returnsNull),
        cmocka_unit_test(destroyContainers_FirstItemPassedIn_returnsNullOBjectsFreed),
        //Container - Get item from container
        cmocka_unit_test(getFromContainer_ListNullReturnsNull),
        cmocka_unit_test(getFromContainer_ListContainsItem_ItemRetrieved),
        //Container - Remove container
        cmocka_unit_test(removeFromContainer_ListNotEmpty_ItemRemovedProperly),
        cmocka_unit_test(removeContainer__ListNullEntryNull__returnsNull),
        cmocka_unit_test(removeContainer__ListNullEntryNotNull__returnsNull),
        cmocka_unit_test(removeContainer__EntryNull__returnsUnchagedList),
        cmocka_unit_test(removeContainer__EntryNotInList_returnsReferenceListUnchanged),
        cmocka_unit_test(removeContainer__EntryInList__returnsReferenceListChanged),
        cmocka_unit_test(removeContainer__EntryInListListOfSizeOne_returnsNull),
        cmocka_unit_test(removeContainer_EntryInMiddle_ReturnsReferenceListChanged),
        //Backpack - Create
        cmocka_unit_test(backpack_create_backpack),
        //Backpack - Destroy
        cmocka_unit_test(backpack_destroy_backpack),
        cmocka_unit_test(backpack_destroy_backpack_backpackNullReturnsNull),
        //Backpack - Add item
        cmocka_unit_test(backpack_add_item_to_backpack_returnsFalseWhenCapacity0),
        cmocka_unit_test(backpack_add_item_to_backpack_returnsTrueWhenCapacityIsNotExceeded),
        cmocka_unit_test(backpack_add_item_to_backpack_returnsFalseWhenMaxCapacityReached),
        //Backpack - Delete item
        cmocka_unit_test(backpack_delete_item_from_backpack_backpackNull_DoesntCrash),
        cmocka_unit_test(backpack_delete_item_from_backpack_ItemNull_DoesntCrash),
        cmocka_unit_test(backpack_delete_item_from_backpack_ItemRemovedWhenInBackpack),
        cmocka_unit_test(backpack_delete_item_from_backpack_ItemNotRemoved_WhenNotInBackpack),
        //Backpack - Get item
        cmocka_unit_test(backpack_get_item_backpackOrNameNull_returnsNull),
        cmocka_unit_test(backpack_get_item_ItemNotInBackpack_returnsNull),
        cmocka_unit_test(backpack_get_item_ItemInBackpack_ItemReturnedBackpackItemsIntact),
        //Room - Create
        cmocka_unit_test(Room_create_room),
        //Room - Destroy
        cmocka_unit_test(Room_destroy_room),
        //Room - Set Exits
        cmocka_unit_test(Room_set_exits_from_room),
        //Room - Show
        cmocka_unit_test(Room_show),
        //Room - Delete item
        cmocka_unit_test(Room_delete_item_from_room),
        //Room - Add Item
        cmocka_unit_test(Room_add_item_to_room),
        //Room - Get Item
        cmocka_unit_test(Room_get_item_from_room),
        //World - Create
        cmocka_unit_test(World_create_world),
        //World - Add room
        cmocka_unit_test(World_add_room_to_world),
        //World - Destroy
        cmocka_unit_test(World_destroy_world),
        //World - Get room
        cmocka_unit_test(World_get_room),
        cmocka_unit_test(Parser_create_parser),
        cmocka_unit_test(Parser_destroy_parser),
        cmocka_unit_test(Parser_parse_input),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}