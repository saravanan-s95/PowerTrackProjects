/*
 * large.c — Layer 3: Large allocations (size > 128 KB) via direct mmap.
 */

#define _GNU_SOURCE
#include "trimalloc_internal.h"
#include "trimalloc.h"
#include "sys.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *large_alloc(size_t size)
{
    size_t overhead = sizeof(struct large_header);
    if (size > SIZE_MAX - overhead - PAGE_SIZE)
        return NULL; /* prevent wrapping */

    size_t mapped = (size_t)align_up(size + overhead, PAGE_SIZE);
    void *mem = sys_alloc(mapped);
    if (!mem)
        return NULL;

    struct large_header *hdr = (struct large_header *)mem;
    hdr->magic       = LARGE_MAGIC;
    hdr->mapped_size = mapped;

    /* Insert into global list */
    hdr->prev = NULL;
    hdr->next = g_allocator.large_allocations;
    if (g_allocator.large_allocations)
        g_allocator.large_allocations->prev = hdr;
    g_allocator.large_allocations = hdr;

    return (char *)mem + overhead;
}

void large_free(void *ptr)
{
    struct large_header *hdr = (struct large_header *)((char *)ptr - sizeof(struct large_header));

    if (hdr->magic != LARGE_MAGIC) {
        fprintf(stderr, "trimalloc: large_free: bad magic 0x%llx\n", (unsigned long long)hdr->magic);
        abort();
    }

    size_t mapped = hdr->mapped_size;

    /* Remove from global list */
    if (hdr->prev)
        hdr->prev->next = hdr->next;
    else
        g_allocator.large_allocations = hdr->next;

    if (hdr->next)
        hdr->next->prev = hdr->prev;

    sys_free(hdr, mapped);
}

void *large_realloc(void *ptr, size_t new_size)
{
    struct large_header *hdr = (struct large_header *)((char *)ptr - sizeof(struct large_header));

    if (hdr->magic != LARGE_MAGIC) {
        fprintf(stderr, "trimalloc: large_realloc: bad magic 0x%llx\n", (unsigned long long)hdr->magic);
        abort();
    }

    size_t old_mapped = hdr->mapped_size;
    size_t overhead = sizeof(struct large_header);
    if (new_size > SIZE_MAX - overhead - PAGE_SIZE)
        return NULL;

    size_t new_mapped = (size_t)align_up(new_size + overhead, PAGE_SIZE);

    /* Return same block if mapping size hasn't changed page count */
    if (new_mapped == old_mapped) {
        return ptr;
    }

    void *new_mem = sys_realloc(hdr, old_mapped, new_mapped);
    if (!new_mem)
        return NULL;

    struct large_header *new_hdr = (struct large_header *)new_mem;
    new_hdr->mapped_size = new_mapped;

    /* Update list pointers if block moved */
    if (new_mem != hdr) {
        if (new_hdr->prev)
            new_hdr->prev->next = new_hdr;
        else
            g_allocator.large_allocations = new_hdr;
            
        if (new_hdr->next)
            new_hdr->next->prev = new_hdr;
    }

    return (char *)new_mem + overhead;
}
