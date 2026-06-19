/*
 * sys_windows.c — Windows implementation of the OS Abstraction Layer
 */

#ifdef _WIN32

#include "sys.h"
#include <windows.h>
#include <string.h>

void *sys_alloc(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void sys_free(void *ptr, size_t size)
{
    (void)size;
    VirtualFree(ptr, 0, MEM_RELEASE);
}

void *sys_realloc(void *ptr, size_t old_size, size_t new_size)
{
    /* Windows lacks mremap; fallback to copy */
    void *new_mem = sys_alloc(new_size);
    if (!new_mem)
        return NULL;

    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy(new_mem, ptr, copy_size);
    sys_free(ptr, old_size);

    return new_mem;
}

#endif /* _WIN32 */
