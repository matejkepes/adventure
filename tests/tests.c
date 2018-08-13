#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "command.h"

static void test_create_command(void ** state)
{
    struct command * command = create_command("TEST_COMMAND", "TEST_COMMAND_DESC", ".*", (size_t) 0);

    assert_true(command->nmatch == (size_t) 0);
}

int main(void) 
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_create_command)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}