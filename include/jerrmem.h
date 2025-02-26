#ifndef JERRMEM_H
#define JERRMEM_H

#include <stddef.h>

#define HEAP_SIZE 1024 * 1024  // 1MB heap
void *malloc(size_t size);
void free(void *ptr);

#endif
