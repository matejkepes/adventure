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

static void createCommand_noMatches_returnsCommandsWithGroupsEqualsNull(void ** state)
{
    COMMAND * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 0);

    assert_true(command->nmatch == (size_t) 0);
    assert_true(strcmp(command->name, "TEST_COMMAND") == 0);
    assert_true(strcmp(command->description, "TEST_COMMAND_DESC") == 0);
    assert_true(command->groups == NULL);
}

static void createCommand_withMatches_returnsCommandsWithGroupsNotNull(void ** state)
{
    COMMAND * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 5);

    assert_true(command->nmatch == (size_t) 5);
    assert_true(strcmp(command->name, "TEST_COMMAND") == 0);
    assert_true(strcmp(command->description, "TEST_COMMAND_DESC") == 0);
    assert_true(command->groups != NULL);
}

static void test_destroy_command(void ** state)
{
    //SETUP
    COMMAND * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 0);

    //TEST
    destroy_command(command);

    //Assertion
    assert_true(command->name == NULL);
    assert_true(command->description == NULL);
}   


static void createItem_allocatedProperly(void ** state) 
{
    ITEM * item = create_item("SWORD", "MIGHTY SWORD", 0x0000);

    assert_true(strcmp(item->name, "SWORD") == 0);
    assert_true(strcmp(item->description, "MIGHTY SWORD") == 0);
    assert_true(item->properties == 0x0000);
}

static void createContainer_firstItemNull_returnsNewContainer(void ** state) {
    ITEM * item = create_item("SWORD", "MIGHTY SWORD", 0x0000);
    struct container * container = NULL;
    
    container = create_container(NULL, TYPE_ITEM, item);

    //  State we're testing for
    //  ############
    //  # ITEM_ONE # ---> NULL
    //  ############

    assert_true(container != NULL);
    assert_true(container->next == NULL);
    assert_true(container->type == TYPE_ITEM);
}

static void createContainer_firstItemNotNull_returnsNewContainerItemIsNewHead(void ** state) {
    //Setup first container item
    ITEM * itemOne = create_item("ITEM_ONE", "MIGHTY SWORD", 0x0000);
    struct container * first = (struct container * ) malloc(sizeof(struct container));
    first->item = itemOne;

    //  After setup
    //  ############
    //  # ITEM_ONE # ---> NULL
    //  ############

    struct container * container = NULL;
    ITEM * itemTwo = create_item("ITEM_TWO", "MIGHTY SWORD", 0x0000);
    
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

int main(void) 
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(createCommand_noMatches_returnsCommandsWithGroupsEqualsNull),
        cmocka_unit_test(createCommand_withMatches_returnsCommandsWithGroupsNotNull),
        cmocka_unit_test(test_destroy_command),
        cmocka_unit_test(createItem_allocatedProperly),
        cmocka_unit_test(createContainer_firstItemNull_returnsNewContainer),
        cmocka_unit_test(createContainer_firstItemNotNull_returnsNewContainerItemIsNewHead)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}