#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "command.h"
#include "item.h"

static void createCommand_noMatches_returnsCommandsWithGroupsEqualsNull(void ** state)
{
    struct command * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 0);

    assert_true(command->nmatch == (size_t) 0);
    assert_true(strcmp(command->name, "TEST_COMMAND") == 0);
    assert_true(strcmp(command->description, "TEST_COMMAND_DESC") == 0);
    assert_true(command->groups == NULL);
}

static void createCommand_withMatches_returnsCommandsWithGroupsNotNull(void ** state)
{
    struct command * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 5);

    assert_true(command->nmatch == (size_t) 5);
    assert_true(strcmp(command->name, "TEST_COMMAND") == 0);
    assert_true(strcmp(command->description, "TEST_COMMAND_DESC") == 0);
    assert_true(command->groups != NULL);
}

static void test_destroy_command(void ** state)
{
    //SETUP
    struct command * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 0);

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

int main(void) 
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(createCommand_noMatches_returnsCommandsWithGroupsEqualsNull),
        cmocka_unit_test(createCommand_withMatches_returnsCommandsWithGroupsNotNull),
        cmocka_unit_test(test_destroy_command),
        cmocka_unit_test(createItem_allocatedProperly)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}