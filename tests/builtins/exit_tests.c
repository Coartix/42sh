#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "alloc.h"
#include "exit.h"

/*
####### All tests are working with deleting exit() line on exit.c file
####### Criterion can not handle easyly exit() signal returned
######################################################################

Test(my_exit, exit_test_1, .disabled = false)
{
    int result = my_exit("exit 1");
    int expected = 1;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_exit, exit_test_1_spaces, .disabled = false)
{
    int result = my_exit("exit                    1");
    int expected = 1;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_exit, exit_test_1_spaces_2, .disabled = false)
{
    int result = my_exit("exit                    1       ");
    int expected = 1;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_exit, exit_test_more_than_255, .disabled = false)
{
    int result = my_exit("exit 256");
    int expected = 256;
    cr_assert_eq(result, expected);
    destroy_global();
}

*/
