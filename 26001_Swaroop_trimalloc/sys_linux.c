/*
 * sys_linux.c — Linux implementation of the OS Abstraction Layer
 */

#define _GNU_SOURCE
#include "sys.h"
#include <sys/mman.h>

void *sys_alloc(size_t size)
{
    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED)
        return NULL;
    return mem;
}

void sys_free(void *ptr, size_t size)
{
    munmap(ptr, size);
}

void *sys_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_mem = mremap(ptr, old_size, new_size, MREMAP_MAYMOVE);
    if (new_mem == MAP_FAILED)
        return NULL;
    return new_mem;
}
