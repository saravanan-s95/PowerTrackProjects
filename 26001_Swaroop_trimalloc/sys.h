/*
 * sys.h — OS Abstraction Layer (OSAL) for trimalloc.
 */

#ifndef TRIMALLOC_SYS_H
#define TRIMALLOC_SYS_H

#include <stddef.h>

/* Allocates raw memory pages from the OS. Returns NULL on failure. */
void *sys_alloc(size_t size);

/* Releases memory pages back to the OS. */
void sys_free(void *ptr, size_t size);

/* Resizes an existing memory mapping. Returns NULL on failure. */
void *sys_realloc(void *ptr, size_t old_size, size_t new_size);

#endif /* TRIMALLOC_SYS_H */
