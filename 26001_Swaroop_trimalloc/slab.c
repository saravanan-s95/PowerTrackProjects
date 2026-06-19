/*
 * slab.c — Layer 1: Small object slab allocator (size <= 2048 bytes).
 */

#define _GNU_SOURCE
#include "trimalloc_internal.h"
#include "trimalloc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Remove page from list */
static void page_list_remove(struct slab_page** head, struct slab_page* page)
{
    if (page->prev)
        page->prev->next = page->next;
    else
        *head = page->next;

    if (page->next)
        page->next->prev = page->prev;

    page->next = NULL;
    page->prev = NULL;
}

/* Prepend page */
static void page_list_push_front(struct slab_page **head, struct slab_page  *page)
{
    page->prev = NULL;
    page->next = *head;
    if (*head)
        (*head)->prev = page;
    *head = page;
}

static struct slab_page *slab_page_new(uint16_t class_id)
{
    void *raw = arena_alloc_page();
    if (!raw)
        return NULL;

    struct slab_page *page = (struct slab_page *)raw;
    memset(page, 0, sizeof(*page));
    page->magic    = SLAB_MAGIC;
    page->class_id = class_id;

    uint16_t cls_size = class_sizes[class_id];
    void    *obj_base = slab_object_base(page);
    size_t   usable   = PAGE_SIZE - (size_t)((uintptr_t)obj_base - (uintptr_t)page);

    page->total_slots = (uint16_t)(usable / cls_size);
    if (page->total_slots > MAX_BITMAP_WORDS * 64)
        page->total_slots = MAX_BITMAP_WORDS * 64;      

    page->free_count = page->total_slots;

    /* Init bitmap */
    memset(page->alloc_bitmap, 0, sizeof(page->alloc_bitmap));
    for (uint16_t i = 0; i < page->total_slots; i++)
        bitmap_set_free(page, i);

    /* Init freelist */
    page->freelist_head = NULL;
    for (int i = (int)page->total_slots - 1; i >= 0; i--)
    {
        struct free_object *obj = (struct free_object *)((char *)obj_base + (size_t)i * cls_size);
        obj->next           = page->freelist_head;
        page->freelist_head = obj;
    }

    page->next = NULL;
    page->prev = NULL;

    return page;
}

/* Verify freelist next pointer bounds */
static inline void validate_freelist_head(struct slab_page *page)
{
    if (page->freelist_head) {
        uintptr_t head = (uintptr_t)page->freelist_head;
        uintptr_t lo   = (uintptr_t)slab_object_base(page);
        uintptr_t hi   = (uintptr_t)page + PAGE_SIZE;
        if (head < lo || head >= hi) {
            fprintf(stderr, "trimalloc: freelist corruption: node %p outside page [%p, %p)\n",
                    (void *)page->freelist_head, (void *)lo, (void *)hi);
            abort();
        }
    }
}

void slab_init(void)
{
    /* Build size class lookup table */
    int cls = 0;
    for (size_t sz = 0; sz <= SLAB_MAX; sz++) {
        while (cls < NUM_CLASSES - 1 && class_sizes[cls] < sz)
            cls++;
        g_allocator.size_to_class[sz] = (uint8_t)cls;
    }

    for (int i = 0; i < NUM_CLASSES; i++) {
        g_allocator.classes[i].object_size   = class_sizes[i];
        g_allocator.classes[i].active_page   = NULL;
        g_allocator.classes[i].partial_pages = NULL;
        g_allocator.classes[i].full_pages    = NULL;
    }
}

void *slab_alloc(size_t size)
{
    if (size > SLAB_MAX)
        return NULL;
    if (size == 0)
        size = 1;

    uint8_t  class_id = g_allocator.size_to_class[size];
    struct size_class *cls = &g_allocator.classes[class_id];
    struct slab_page *page = cls->active_page;

    if (!page || page->free_count == 0) {
        page = cls->partial_pages;
        if (page) {
            page_list_remove(&cls->partial_pages, page);
        } else {
            page = slab_page_new(class_id);
            if (!page)
                return NULL;
        }
        cls->active_page = page;
    }

    /* Pop freelist */
    struct free_object *obj = page->freelist_head;
    if (!obj) {
        fprintf(stderr, "trimalloc: freelist empty but free_count=%u\n", page->free_count);
        abort();
    }
    page->freelist_head = obj->next;
    validate_freelist_head(page);

    /* Mark allocated in bitmap */
    int slot = slab_slot_index(page, obj);
    if (slot < 0) {
        fprintf(stderr, "trimalloc: freelist contains invalid pointer\n");
        abort();
    }
    bitmap_set_allocated(page, (uint16_t)slot);

    page->free_count--;

    /* Transition if full */
    if (page->free_count == 0) {
        page_list_push_front(&cls->full_pages, page);
        
        if (cls->partial_pages) {
            cls->active_page = cls->partial_pages;
            page_list_remove(&cls->partial_pages, cls->active_page);
        } else {
            cls->active_page = NULL;
        }
    }

    return (void *)obj;
}

void slab_free(void *ptr, struct arena *arena, size_t page_idx)
{
    (void)arena;
    (void)page_idx;

    struct slab_page *page = (struct slab_page *)align_down((uintptr_t)ptr, PAGE_SIZE);

    if (page->magic != SLAB_MAGIC) {
        fprintf(stderr, "trimalloc: slab_free: bad magic\n");
        abort();
    }

    int slot = slab_slot_index(page, ptr);
    if (slot < 0) {
        fprintf(stderr, "trimalloc: slab_free: invalid pointer %p\n", ptr);
        abort();
    }

    /* Double free check */
    if (bitmap_is_free(page, (uint16_t)slot)) {
        fprintf(stderr, "trimalloc: double free detected on %p (slot %d)\n", ptr, slot);
        abort();
    }

    uint16_t class_id = page->class_id;
    struct size_class *cls = &g_allocator.classes[class_id];
    bool was_full = (page->free_count == 0);

    bitmap_set_free(page, (uint16_t)slot);

    struct free_object *obj = (struct free_object *)ptr;
    obj->next           = page->freelist_head;
    page->freelist_head = obj;

    page->free_count++;

    if (was_full) {
        page_list_remove(&cls->full_pages, page);
        if (!cls->active_page) {
            cls->active_page = page;
        } else {
            page_list_push_front(&cls->partial_pages, page);
        }
    }

    if (page->free_count == page->total_slots) {
        /* Release page if we already have an active one */
        bool keep_warm = (cls->active_page == page && !cls->partial_pages);

        if (!keep_warm) {
            if (cls->active_page == page) {
                cls->active_page = cls->partial_pages;
                if (cls->active_page) {
                    page_list_remove(&cls->partial_pages, cls->active_page);
                }
            } else {
                page_list_remove(&cls->partial_pages, page);
            }

            size_t pidx;
            struct arena *a = find_arena(page, &pidx);
            if (a) {
                page->magic = 0;  /* poison magic */
                arena_free_page(a, pidx);
            }
        }
    }
}
