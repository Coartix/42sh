#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/attributes.h>

/**
 * @brief Different pointers that we need to run our garbage collector.
 *
 */
static void **g_alloc_ptr;
static size_t g_capacity = 1;
static size_t g_size = 0;

/**
 * @brief Check is the pointer is already in the list.
 *
 * @param alloc
 * @return int
 */
static int check_in_alloc(const void *alloc)
{
    for (size_t i = 0; i < g_size; i++)
    {
        if (g_alloc_ptr[i] == alloc)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Set an intern pointer to NULL. So fsanitize could detect the memory
 * leaks.
 *
 */
void destroy_global()
{
    free(g_alloc_ptr);
    g_alloc_ptr = NULL;
}

/**
 * @brief Free all the allocated memory remaining in the program. Calling it at
 * the end of the program is a goog utilisation.
 *
 */
void free_all()
{
    for (size_t i = 0; i < g_size; i++)
    {
        if (g_alloc_ptr[i] != NULL)
            free(g_alloc_ptr[i]);
    }
    free(g_alloc_ptr);
    g_alloc_ptr = NULL;
    g_capacity = 1;
    g_size = 0;
}

/**
 * @brief Set internaly the pointer to free and free it. The function MUST be
 * called instead of free().
 *
 * @param alloc
 */
void free_alloc(const void *alloc)
{
    if (alloc == NULL)
        return;

    int i = check_in_alloc(alloc);
    if (i != -1)
    {
        free(g_alloc_ptr[i]);
        g_alloc_ptr[i] = NULL;
    }
    else
        fprintf(stderr, "You tried to double free\n");
}

/**
 * @brief Internal function for the garbage collector.
 *
 * @param alloc
 */
static void add_alloc_ptr(void *alloc)
{
    if (g_size >= g_capacity - 1)
    {
        g_alloc_ptr = realloc(g_alloc_ptr, sizeof(void *) * g_capacity * 2);
        if (g_alloc_ptr == NULL)
            abort();
        g_capacity *= 2;
    }
    g_alloc_ptr[g_size] = alloc;
    g_size++;
}

/**
 * @brief Like malloc, but calls abort() in case of failure.
 *
 */
void *__malloc __alloc_size(1) xmalloc(size_t size)
{
    if (size == 0)
        return NULL;
    void *res = malloc(size);
    if (res == NULL && size != 0)
        abort();
    add_alloc_ptr(res);
    return res;
}

/**
 * @brief Like realloc, but calls abort() in case of failure.
 *
 */
void *__alloc_size(2) xrealloc(void *ptr, size_t size)
{
    void *res = realloc(ptr, size);
    if (res == NULL && size != 0)
        abort();
    if (ptr != res)
    {
        int i = check_in_alloc(ptr);
        if (i != -1)
        {
            g_alloc_ptr[i] = NULL;
        }
        add_alloc_ptr(res);
    }
    else if (size == 0)
        res = NULL;
    return res;
}

/**
 * @brief Like malloc, but initializes memory to zero and calls abort() in case
 * of failure.
 *
 */
void *__malloc __alloc_size(1) zalloc(size_t size)
{
    if (size == 0)
        return NULL;
    void *res = xmalloc(size);
    memset(res, 0, size);
    return res;
}
