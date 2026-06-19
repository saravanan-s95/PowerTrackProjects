/*
 * trimalloc.c — Top-level dispatcher
 */

#define _GNU_SOURCE
#include "trimalloc_internal.h"
#include "trimalloc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct allocator_state g_allocator;

void trimalloc_init(void)
{
    if (g_allocator.initialized)
        return;

    memset(&g_allocator, 0, sizeof(g_allocator));

    slab_init();
    page_alloc_init();

    g_allocator.initialized = 1;
}

static inline void ensure_init(void)
{
    if (!g_allocator.initialized)
        trimalloc_init();
}

void *trimalloc(size_t size)
{
    ensure_init();

    if (size == 0)
        size = 1;   /* min size class */

    if (size <= SLAB_MAX)
        return slab_alloc(size);

    if (size <= PAGE_ALLOC_MAX)
        return page_alloc(size);

    return large_alloc(size);
}

/*
 * Returns:
 *    'S' — slab
 *    'R' — run
 *    'L' — large
 *    'F' — freed
 *    '?' — corrupt
 */
static char identify_ptr(void *ptr, struct arena **out_arena, size_t *out_page_idx)
{
    size_t page_idx;
    struct arena *arena = find_arena(ptr, &page_idx);

    if (arena) {
        uint8_t type = arena->page_map[page_idx];
        *out_arena   = arena;

        if (type == PAGE_SLAB) {
            *out_page_idx = page_idx;
            return 'S';
        }

        if (type == PAGE_RUN_START) {
            *out_page_idx = page_idx;
            return 'R';
        }

        if (type == PAGE_RUN_CONT) {
            /* Walk back to find the run start */
            size_t idx = page_idx;
            while (idx > 0 && arena->page_map[idx] == PAGE_RUN_CONT)
                idx--;
            if (arena->page_map[idx] == PAGE_RUN_START) {
                *out_page_idx = idx;
                return 'R';
            }
            return '?';
        }

        if (type == PAGE_FREE) {
            *out_page_idx = page_idx;
            return 'F';
        }

        return '?';
    }

    *out_arena    = NULL;
    *out_page_idx = 0;
    return 'L';
}

void trifree(void *ptr)
{
    if (!ptr)
        return;

    ensure_init();

    struct arena *arena;
    size_t page_idx;
    char kind = identify_ptr(ptr, &arena, &page_idx);

    switch (kind) 
    {
    case 'S':
        slab_free(ptr, arena, page_idx);
        break;

    case 'R':
        page_free(ptr, arena, page_idx);
        break;

    case 'L':
        large_free(ptr);
        break;

    case 'F':
        fprintf(stderr, "trimalloc: double free detected on %p\n", ptr);
        abort();

    default:
        fprintf(stderr, "trimalloc: trifree: unrecognised pointer %p\n", ptr);
        abort();
    }
}

static size_t usable_size(void *ptr, char kind, struct arena *arena, size_t page_idx)
{
    switch (kind) {
    case 'S': {
        struct slab_page *page = (struct slab_page *)align_down((uintptr_t)ptr, PAGE_SIZE);
        return class_sizes[page->class_id];
    }
    case 'R': {
        void *base = arena_page_addr(arena, page_idx);
        struct run_header *hdr = (struct run_header *)base;
        size_t hdr_offset = (size_t)align_up(sizeof(struct run_header), OBJECT_ALIGN);
        return (size_t)hdr->page_count * PAGE_SIZE - hdr_offset;
    }
    case 'L': {
        struct large_header *hdr = (struct large_header *)((char *)ptr - sizeof(struct large_header));
        return hdr->mapped_size - sizeof(struct large_header);
    }
    default:
        return 0;
    }
}

void *trirealloc(void *ptr, size_t new_size)
{
    if (!ptr)
        return trimalloc(new_size);

    if (new_size == 0) {
        trifree(ptr);
        return NULL;
    }

    ensure_init();

    struct arena *arena;
    size_t page_idx;
    char kind = identify_ptr(ptr, &arena, &page_idx);
    size_t old_size = usable_size(ptr, kind, arena, page_idx);

    if (new_size <= old_size) {
        /* Migrate if new size is much smaller (>4x reduction) */
        if (old_size > 4 * new_size) {
            void *new_ptr = trimalloc(new_size);
            if (new_ptr) {
                memcpy(new_ptr, ptr, new_size);
                trifree(ptr);
                return new_ptr;
            }
        }
        return ptr;
    }

    if (kind == 'L')
        return large_realloc(ptr, new_size);

    void *new_ptr = trimalloc(new_size);
    if (!new_ptr)
        return NULL;

    memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
    trifree(ptr);
    return new_ptr;
}

void *tricalloc(size_t nmemb, size_t size)
{
    if (nmemb == 0 || size == 0)
        return trimalloc(0);

    /* Multiplication overflow check */
    if (nmemb > SIZE_MAX / size)
        return NULL;

    size_t total = nmemb * size;
    void  *ptr   = trimalloc(total);
    if (ptr)
        memset(ptr, 0, total);
    return ptr;
}

#ifdef TRIMALLOC_DEBUG
int trimalloc_validate(void)
{
    ensure_init();
    return validate_all();
}
#endif

void trimalloc_report_leaks(void)
{
    int leaks = 0;
    fprintf(stderr, "\n=========================================\n");
    fprintf(stderr, "         TRIMALLOC LEAK DETECTOR           \n");
    fprintf(stderr, "=========================================\n");

    /* Slab/runs */
    for (int ai = 0; ai < g_allocator.arena_count; ai++) {
        struct arena *arena = g_allocator.arenas[ai];
        for (size_t i = 0; i < arena->usable_pages; i++) 
        {
            uint8_t type = arena->page_map[i];

            if (type == PAGE_SLAB) {
                struct slab_page *page = (struct slab_page *)arena_page_addr(arena, i);
                uint16_t cls_size = class_sizes[page->class_id];
                void *obj_base = slab_object_base(page);

                for (uint16_t slot = 0; slot < page->total_slots; slot++) {
                    if (!bitmap_is_free(page, slot)) {
                        void *ptr = (char *)obj_base + (slot * cls_size);
                        fprintf(stderr, "LEAK DETECTED: Slab object at %p (%u bytes)\n", ptr, cls_size);
                        leaks++;
                    }
                }
            }
            else if (type == PAGE_RUN_START) {
                void *base = arena_page_addr(arena, i);
                struct run_header *hdr = (struct run_header *)base;
                
                size_t hdr_offset = (size_t)align_up(sizeof(struct run_header), OBJECT_ALIGN);
                void *ptr = (char *)base + hdr_offset;
                size_t bytes = hdr->page_count * PAGE_SIZE - hdr_offset;
                
                fprintf(stderr, "LEAK DETECTED: Page-run at %p (~%zu bytes, %u pages)\n", ptr, bytes, hdr->page_count);
                leaks++;
                
                i += hdr->page_count - 1;
            }
        }
    }

    /* Large allocations */
    struct large_header *cur = g_allocator.large_allocations;
    while (cur) {
        void *ptr = (char *)cur + sizeof(struct large_header);
        size_t bytes = cur->mapped_size - sizeof(struct large_header);
        fprintf(stderr, "LEAK DETECTED: Large allocation at %p (~%zu bytes)\n", ptr, bytes);
        leaks++;
        cur = cur->next;
    }

    if (leaks == 0) {
        fprintf(stderr, "No memory leaks detected.\n");
    } else {
        fprintf(stderr, "-----------------------------------------\n");
        fprintf(stderr, "TOTAL LEAKS: %d\n", leaks);
    }
    fprintf(stderr, "=========================================\n\n");
}
