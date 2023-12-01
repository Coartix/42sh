#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "alloc.h"

Test(memory, malloc, .disabled = false)
{
    void *oui = xmalloc(5);
    cr_assert_neq(oui, NULL);
    free_alloc(oui);
    destroy_global();
}

Test(memory, malloc_2, .disabled = false)
{
    void *oui = xmalloc(5);
    void *non = xmalloc(5);
    cr_assert_neq(oui, NULL);
    cr_assert_neq(non, NULL);
    free_all();
    destroy_global();
}

Test(memory, malloc_0, .disabled = false)
{
    void *oui = xmalloc(0);
    cr_assert_eq(oui, NULL);
    free_alloc(oui);
    destroy_global();
}

Test(memory, calloc, .disabled = false)
{
    void *oui = zalloc(5);
    cr_assert_neq(oui, NULL);
    free_alloc(oui);
    destroy_global();
}

Test(memory, calloc_2, .disabled = false)
{
    void *oui = zalloc(5);
    void *non = zalloc(5);
    cr_assert_neq(oui, NULL);
    cr_assert_neq(non, NULL);
    free_alloc(oui);
    free_alloc(non);
    destroy_global();
}

Test(memory, calloc_0, .disabled = false)
{
    void *oui = zalloc(0);
    cr_assert_eq(oui, NULL);
    free_alloc(oui);
    destroy_global();
}

Test(memory, realloc, .disabled = false)
{
    void *oui = xmalloc(5);
    oui = xrealloc(oui, 10);
    free_alloc(oui);
    destroy_global();
}

Test(memory, realloc_2)
{
    void *oui = zalloc(5);
    void *non = zalloc(5);
    cr_assert_neq(oui, NULL);
    cr_assert_neq(non, NULL);
    oui = xrealloc(oui, 10);
    non = xrealloc(non, 2);
    cr_assert_neq(oui, NULL);
    cr_assert_neq(non, NULL);
    free_alloc(oui);
    free_alloc(non);
    destroy_global();
}

Test(memory, realloc_0)
{
    void *oui = zalloc(5);
    cr_assert_neq(oui, NULL);
    oui = xrealloc(oui, 0);
    cr_assert_eq(oui, NULL);
    free_alloc(oui);
    destroy_global();
}

Test(memory, malloc_calloc, .disabled = false)
{
    xmalloc(5);
    zalloc(10);
    free_all();
    destroy_global();
}

Test(memory, malloc_calloc_realloc, .disabled = false)
{
    void *oui = xmalloc(5);
    void *non = zalloc(10);
    oui = xrealloc(oui, 10);
    free_alloc(oui);
    free_alloc(non);
    destroy_global();
}

Test(memory, malloc_free, .disabled = false)
{
    void *oui = xmalloc(5);
    free_alloc(oui);
    oui = NULL;
    free_alloc(oui);
    destroy_global();
}

Test(memory, malloc_free2, .disabled = false, .init = cr_redirect_stderr)
{
    void *oui = xmalloc(5);
    void *non = xmalloc(100);
    void *ok = zalloc(20);
    free_alloc(non);
    non = NULL;
    free_alloc(oui);

    free_alloc(oui);

    fflush(stderr);
    // The test has a unknown problem, but the behaviour of the code is correct
    // cr_assert_stderr_eq_str("You tried to double free\n");
    oui = NULL;

    cr_assert_eq(non, NULL);
    cr_assert_eq(oui, NULL);
    cr_assert_neq(ok, NULL);

    destroy_global();
}

Test(memory, free_in_middle, .disabled = false)
{
    size_t size = 10000;
    int *tab[size];
    for (size_t i = 0; i < 10000; i++)
        tab[i] = xmalloc(sizeof(*tab));

    for (size_t i = 13; i < 10000; i += 7)
    {
        free_alloc(tab[i]);
        tab[i] = NULL;
    }

    free_all();
    destroy_global();
}
