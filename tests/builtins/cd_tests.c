#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "alloc.h"
#include "cd.h"

Test(my_cd, cd_test_pp, .disabled = false)
{
    int result = my_cd("cd ..");
    int expected = 0;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_cd, cd_test_minus, .disabled = false)
{
    int result = my_cd("cd -");
    int expected = 0;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_cd, cd_test_without, .disabled = false)
{
    int result = my_cd("cd");
    int expected = 0;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_cd, cd_test_with_spaces, .disabled = false)
{
    int result = my_cd("cd                      ");
    int expected = 0;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_cd, cd_test_no_file, .disabled = false, .init = cr_redirect_stderr)
{
    int result = my_cd("cd folder_no_exists");
    int expected = 0;
    cr_assert_eq(result, expected);
    fflush(stderr);
    cr_assert_stderr_eq_str(
        "cd: folder_no_exists: No such file or directory\n");
    destroy_global();
}

Test(my_cd, cd_test_several_pp, .disabled = false)
{
    int result = my_cd("cd ../../../../../../../..");
    int expected = 0;
    cr_assert_eq(result, expected);
    destroy_global();
}

Test(my_cd, cd_test_failed_pp, .disabled = false, .init = cr_redirect_stderr)
{
    int result = my_cd("cd .../.");
    int expected = 0;
    cr_assert_eq(result, expected);
    fflush(stderr);
    cr_assert_stderr_eq_str("cd: .../.: No such file or directory\n");
    destroy_global();
}

Test(my_cd, cd_test_several_pp_nonsense, .disabled = false)
{
    int result = my_cd("cd ./.././.././../../.");
    int expected = 0;
    cr_assert_eq(result, expected);
    destroy_global();
}
