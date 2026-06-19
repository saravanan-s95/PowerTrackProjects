/*
 * page_alloc.c — Layer 2: Page-run arena allocator (2048 < size <= 128 KB).
 */

#define _GNU_SOURCE
#include "trimalloc_internal.h"
#include "trimalloc.h"
#include "sys.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Register arena in sorted order */
void arena_register(struct arena *arena)
{
    if (g_allocator.arena_count >= MAX_ARENAS) {
        fprintf(stderr, "trimalloc: exceeded maximum arena count (%d)\n", MAX_ARENAS);
        abort();
    }

    uintptr_t base = (uintptr_t)arena;
    int pos = g_allocator.arena_count;

    for (int i = 0; i < g_allocator.arena_count; i++) {
        if ((uintptr_t)g_allocator.arenas[i] > base) {
            pos = i;
            break;
        }
    }

    memmove(&g_allocator.arenas[pos + 1], &g_allocator.arenas[pos], (size_t)(g_allocator.arena_count - pos) * sizeof(struct arena *));
    g_allocator.arenas[pos] = arena;
    g_allocator.arena_count++;
}

/* Unregister arena */
void arena_unregister(struct arena *arena)
{
    for (int i = 0; i < g_allocator.arena_count; i++) {
        if (g_allocator.arenas[i] == arena) {
            memmove(&g_allocator.arenas[i], &g_allocator.arenas[i + 1], (size_t)(g_allocator.arena_count - i - 1) * sizeof(struct arena *));
            g_allocator.arena_count--;
            return;
        }
    }
}

/* Return empty arena to OS, keeping at least one warm */
void try_release_arena(struct arena *arena)
{
    if (g_allocator.arena_count <= 1)
        return;

    if (arena->free_pages != arena->usable_pages)
        return;

    arena->magic = 0;
    arena_unregister(arena);
    sys_free(arena, ARENA_SIZE);
}

static struct arena *arena_new(void)
{
    void *mem = sys_alloc(ARENA_SIZE);
    if (!mem)
        return NULL;

    /* Overlay arena struct on the first page */
    struct arena *arena = (struct arena *)mem;
    arena->magic        = ARENA_MAGIC;
    arena->usable_pages = ARENA_USABLE;
    arena->free_pages   = ARENA_USABLE;

    arena_register(arena);
    return arena;
}

void page_alloc_init(void)
{
    if (g_allocator.arena_count == 0) {
        if (!arena_new()) {
            fprintf(stderr, "trimalloc: failed to create initial arena\n");
            abort();
        }
    }
}

/* Single-page allocation (for slab use) */
void *arena_alloc_page(void)
{
    for (int ai = 0; ai < g_allocator.arena_count; ai++) {
        struct arena *a = g_allocator.arenas[ai];
        if (a->free_pages == 0)
            continue;

        for (size_t i = 0; i < a->usable_pages; i++) {
            if (a->page_map[i] == PAGE_FREE) {
                a->page_map[i] = PAGE_SLAB;
                a->free_pages--;
                return arena_page_addr(a, i);
            }
        }
    }

    struct arena *a = arena_new();
    if (!a)
        return NULL;

    a->page_map[0] = PAGE_SLAB;
    a->free_pages--;
    return arena_page_addr(a, 0);
}

/* Single-page release (for slab use) */
void arena_free_page(struct arena *arena, size_t page_idx)
{
    if (page_idx >= arena->usable_pages) {
        fprintf(stderr, "trimalloc: arena_free_page: page_idx %zu out of range\n", page_idx);
        abort();
    }

    arena->page_map[page_idx] = PAGE_FREE;
    arena->free_pages++;

    /* Zero page memory */
    void *page = arena_page_addr(arena, page_idx);
    memset(page, 0, PAGE_SIZE);

    try_release_arena(arena);
}

/* Page-run allocation (2048 < size <= 128 KB) */
void *page_alloc(size_t size)
{
    if (size == 0 || size > PAGE_ALLOC_MAX)
        return NULL;

    size_t hdr_offset = (size_t)align_up(sizeof(struct run_header), OBJECT_ALIGN);
    if (size > SIZE_MAX - hdr_offset) {
        return NULL;
    }
    size_t total    = size + hdr_offset;
    size_t need     = (total + PAGE_SIZE - 1) / PAGE_SIZE;

    /* First-fit search for contiguous pages */
    for (int ai = 0; ai < g_allocator.arena_count; ai++) {
        struct arena *a = g_allocator.arenas[ai];
        if (a->free_pages < need)
            continue;

        size_t run_start = 0;
        size_t run_len   = 0;

        for (size_t i = 0; i < a->usable_pages; i++) {
            if (a->page_map[i] == PAGE_FREE) {
                if (run_len == 0)
                    run_start = i;
                run_len++;
                if (run_len == need) {
                    a->page_map[run_start] = PAGE_RUN_START;
                    for (size_t j = run_start + 1; j < run_start + need; j++)
                        a->page_map[j] = PAGE_RUN_CONT;

                    a->free_pages -= need;

                    void *base = arena_page_addr(a, run_start);
                    struct run_header *hdr = (struct run_header *)base;
                    hdr->page_count = (uint32_t)need;
                    hdr->flags      = 0;

                    return (char *)base + hdr_offset;
                }
            } else {
                run_len = 0;
            }
        }
    }

    struct arena *a = arena_new();
    if (!a)
        return NULL;

    if (need > a->usable_pages)
        return NULL;

    a->page_map[0] = PAGE_RUN_START;
    for (size_t j = 1; j < need; j++)
        a->page_map[j] = PAGE_RUN_CONT;

    a->free_pages -= need;

    void *base = arena_page_addr(a, 0);
    struct run_header *hdr = (struct run_header *)base;
    hdr->page_count = (uint32_t)need;
    hdr->flags      = 0;

    return (char *)base + hdr_offset;
}

/* Page-run free */
void page_free(void *ptr, struct arena *arena, size_t page_idx)
{
    (void)ptr;

    if (arena->page_map[page_idx] != PAGE_RUN_START) {
        fprintf(stderr, "trimalloc: page_free: page %zu is not RUN_START\n", page_idx);
        abort();
    }

    void *base = arena_page_addr(arena, page_idx);
    struct run_header *hdr = (struct run_header *)base;
    uint32_t count = hdr->page_count;

    if (count == 0 || page_idx + count > arena->usable_pages) {
        fprintf(stderr, "trimalloc: page_free: invalid run length %u\n", count);
        abort();
    }

    /* Clear run pages */
    arena->page_map[page_idx] = PAGE_FREE;
    for (uint32_t j = 1; j < count; j++) {
        if (arena->page_map[page_idx + j] != PAGE_RUN_CONT) {
            fprintf(stderr, "trimalloc: page_free: page %zu expected RUN_CONT\n", page_idx + j);
            abort();
        }
        arena->page_map[page_idx + j] = PAGE_FREE;
    }

    arena->free_pages += count;

    /* Zero run memory */
    memset(base, 0, (size_t)count * PAGE_SIZE);

    try_release_arena(arena);
}
