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
    assert_ptr_equal(get_from_container_by_name(third, "ITEM_ONE"), first);
    assert_ptr_equal(get_from_container_by_name(third, "ITEM_TWO"), second);
    assert_ptr_equal(get_from_container_by_name(third, "ITEM_THREE"), third);
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
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);

    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);
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

static void backpack_destroy_backpack_backpackNullReturnsNull(void **state)
{
    assert_null(destroy_backpack(NULL));
}

static void backpack_destroy_backpack(void **state)
{
    const int BACKPACK_CAPACITY = 10;
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);

    //Setup list of three items
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    ITEM *itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    ITEM *itemThree = create_item("ITEM_THREE", "MIGHTY SWORD", 0x0000);
    struct container *first = create_container(NULL, TYPE_ITEM, itemOne);
    struct container *second = create_container(first, TYPE_ITEM, itemTwo);
    struct container *third = create_container(second, TYPE_ITEM, itemThree);

    backpack->items = third;
    backpack->size = 3;

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
    const int BACKPACK_CAPACITY = 0;
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);

    assert_false(add_item_to_backpack(backpack, itemOne));
    assert_null(backpack->items);
    assert_int_equal(0, backpack->size);
}

static void backpack_add_item_to_backpack_returnsTrueWhenCapacityIsNotExceeded(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);
    ITEM *itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);

    assert_true(add_item_to_backpack(backpack, itemOne));
    assert_ptr_equal(backpack->items->item, itemOne);
    assert_int_equal(1, backpack->size);
}

static void backpack_add_item_to_backpack_returnsFalseWhenMaxCapacityReached(void **state)
{
    const int BACKPACK_CAPACITY = 1;
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);
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
    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);
    backpack->size = 1;
    backpack->capacity = BACKPACK_CAPACITY;
    backpack->items = create_container(NULL, TYPE_ITEM, itemOne);

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

    BACKPACK *backpack = create_backpack(BACKPACK_CAPACITY);
    backpack->size = 1;
    backpack->capacity = BACKPACK_CAPACITY;
    backpack->items = create_container(NULL, TYPE_ITEM, itemOne);

    //Delete an item which is not in the backpack
    delete_item_from_backpack(backpack, itemTwo);

    assert_int_equal(backpack->size, 1);
    assert_int_equal(backpack->capacity, BACKPACK_CAPACITY);
    assert_ptr_equal(backpack->items->item, itemOne);
}

static void backpack_get_item_backpackNull_returnsNull(void **state)
{
}

static void backpack_get_item_ItemNotInBackpack_returnsNull(void **state)
{
}

static void backpack_get_item_ItemInBackpack_ItemReturnedBackpackItemsIntact(void **state)
{
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
        cmocka_unit_test(backpack_get_item_backpackNull_returnsNull),
        cmocka_unit_test(backpack_get_item_ItemNotInBackpack_returnsNull),
        cmocka_unit_test(backpack_get_item_ItemInBackpack_ItemReturnedBackpackItemsIntact),

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}