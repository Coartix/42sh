#include <utils/vec.h>

void vec_init(struct vec *vec)
{
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

void vec_destroy(struct vec *vec)
{
    free_alloc(vec->data);
    // this isn't strictly required, but makes debugging a lot easier:
    // the app will crash when using a destroyed vector, instead of writing into
    // freed memory (which may or may not crash the program, but will likely
    // result) in the program crashing at a later time
    vec->data = NULL;
}

static void vec_grow(struct vec *vec)
{
    size_t new_capacity;
    if (vec->capacity == 0)
        new_capacity = 10;
    else
        new_capacity = vec->capacity * 2;

    char *new_data = xrealloc(vec->data, new_capacity);
    vec->data = new_data;
    vec->capacity = new_capacity;
}

void vec_reset(struct vec *vec)
{
    vec->size = 0;
}

void vec_push(struct vec *vec, char c)
{
    if (vec->size == vec->capacity)
        vec_grow(vec);

    vec->data[vec->size++] = c;
}

char *vec_cstring(struct vec *vec)
{
    if (vec->size == 0 || vec->data[vec->size - 1] != '\0')
        vec_push(vec, '\0');
    return vec->data;
}
