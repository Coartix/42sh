#pragma once

#include <stddef.h>
#include <utils/attributes.h>

void free_all();
void free_alloc(const void *alloc);
void destroy_global();

void *__malloc __alloc_size(1) xmalloc(size_t size);

void *__alloc_size(2) xrealloc(void *ptr, size_t size);

void *__malloc __alloc_size(1) zalloc(size_t size);
