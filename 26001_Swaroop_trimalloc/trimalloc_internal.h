#ifndef TRIMALLOC_INTERNAL_H
#define TRIMALLOC_INTERNAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Constants */
#ifdef PAGE_SIZE
#undef PAGE_SIZE
#endif
#define PAGE_SIZE            4096U
#define ARENA_SIZE           (2U * 1024U * 1024U)
#define SLAB_MAX             2048U
#define PAGE_ALLOC_MAX       (128U * 1024U)
#define NUM_CLASSES          24
#define MAX_BITMAP_WORDS     8
#define OBJECT_ALIGN         16U

#define ARENA_PAGES          (ARENA_SIZE / PAGE_SIZE)
#define ARENA_HEADER_PAGES   1U
#define ARENA_USABLE         (ARENA_PAGES - ARENA_HEADER_PAGES)

#define SLAB_MAGIC           0x534C4142U
#define LARGE_MAGIC          0xDEAD4C4FU
#define ARENA_MAGIC          0x41524E41U
#define MAX_ARENAS           256

/* Page map byte values */
#define PAGE_FREE            0x00
#define PAGE_SLAB            0x01
#define PAGE_RUN_START       0x02
#define PAGE_RUN_CONT        0x03

static const uint16_t class_sizes[NUM_CLASSES] = 
{
    16,   32,   48,   64,   80,   96,  112,  128,
   160,  192,  224,  256,
   320,  384,  448,  512,
   640,  768,  896, 1024,
  1280, 1536, 1792, 2048
};

/* Embedded freelist node */
struct free_object {
    struct free_object *next;
};

/* Slab page header */
struct slab_page {
    uint32_t  magic;
    uint16_t  class_id;
    uint16_t  total_slots;
    uint16_t  free_count;
    uint64_t  alloc_bitmap[MAX_BITMAP_WORDS];
    struct free_object  *freelist_head;
    struct slab_page    *next;
    struct slab_page    *prev;
};

/* Page run start header */
struct run_header {
    uint32_t  page_count;
    uint32_t  flags;
};

/* Large allocation header */
struct large_header 
{
    uint64_t                magic;
    size_t                  mapped_size;
    struct large_header *prev;
    struct large_header *next;
};

/* 2MB Arena header */
struct arena 
{
    uint64_t        magic;
    size_t          usable_pages;
    size_t          free_pages;
    uint8_t         page_map[ARENA_USABLE];
};

/* Per-class metadata */
struct size_class {
    struct slab_page *active_page;
    struct slab_page *partial_pages;
    struct slab_page *full_pages;
    uint16_t             object_size;
};

/* Global state */
struct allocator_state {
    struct size_class    classes[NUM_CLASSES];
    uint8_t                 size_to_class[SLAB_MAX + 1];
    struct arena        *arenas[MAX_ARENAS];
    int                     arena_count;
    int                     initialized;
    struct large_header *large_allocations;
};

extern struct allocator_state g_allocator;

/* Declarations */
void   slab_init(void);
void  *slab_alloc(size_t size);
void   slab_free(void *ptr, struct arena *arena, size_t page_idx);

void   page_alloc_init(void);
void  *page_alloc(size_t size);
void   page_free(void *ptr, struct arena *arena, size_t page_idx);
void  *arena_alloc_page(void);
void   arena_free_page(struct arena *arena, size_t page_idx);
void   arena_register(struct arena *arena);
void   arena_unregister(struct arena *arena);
void   try_release_arena(struct arena *arena);

void  *large_alloc(size_t size);
void   large_free(void *ptr);
void  *large_realloc(void *ptr, size_t new_size);

#ifdef TRIMALLOC_DEBUG
int    validate_slab_page(struct slab_page *page);
int    validate_arena(struct arena *arena);
int    validate_all(void);
#endif

/* Bitmap helpers (1 = free, 0 = allocated) */
static inline bool bitmap_is_free(const struct slab_page *page, uint16_t slot)
{
    uint16_t word = slot / 64;
    uint16_t bit  = slot % 64;
    return (page->alloc_bitmap[word] >> bit) & 1U;
}

static inline void bitmap_set_free(struct slab_page *page, uint16_t slot)
{
    uint16_t word = slot / 64;
    uint16_t bit  = slot % 64;
    page->alloc_bitmap[word] |= ((uint64_t)1 << bit);
}

static inline void bitmap_set_allocated(struct slab_page *page, uint16_t slot)
{
    uint16_t word = slot / 64;
    uint16_t bit  = slot % 64;
    page->alloc_bitmap[word] &= ~((uint64_t)1 << bit);
}

static inline uint16_t bitmap_popcount(const struct slab_page *page)
{
    uint16_t count = 0;
    for (int i = 0; i < MAX_BITMAP_WORDS; i++)
        count += (uint16_t)__builtin_popcountll(page->alloc_bitmap[i]);
    return count;
}

static inline uintptr_t align_up(uintptr_t val, uintptr_t align)
{
    return (val + align - 1) & ~(align - 1);
}

static inline uintptr_t align_down(uintptr_t val, uintptr_t align)
{
    return val & ~(align - 1);
}

static inline void *slab_object_base(struct slab_page *page)
{
    uintptr_t after_header = (uintptr_t)page + sizeof(struct slab_page);
    return (void *)align_up(after_header, OBJECT_ALIGN);
}

/* Returns slot index or -1 if misaligned/out of bounds */
static inline int slab_slot_index(struct slab_page *page, void *ptr)
{
    uintptr_t obj_base = (uintptr_t)slab_object_base(page);
    if ((uintptr_t)ptr < obj_base)
        return -1;

    uintptr_t offset   = (uintptr_t)ptr - obj_base;
    uint16_t  cls_size = class_sizes[page->class_id];
    if (offset % cls_size != 0)
        return -1;

    int slot = (int)(offset / cls_size);
    if (slot < 0 || slot >= page->total_slots)
        return -1;

    return slot;
}

/* Binary search for arena containing ptr */
static inline struct arena *find_arena(void *ptr, size_t *out_page_idx)
{
    uintptr_t target = (uintptr_t)ptr;
    int lo = 0, hi = g_allocator.arena_count - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        struct arena *a = g_allocator.arenas[mid];
        uintptr_t base       = (uintptr_t)a;
        uintptr_t data_start = base + PAGE_SIZE;
        uintptr_t data_end   = base + ARENA_SIZE;

        if (target < base) {
            hi = mid - 1;
        } else if (target >= data_end) {
            lo = mid + 1;
        } else if (target >= data_start) {
            if (out_page_idx)
                *out_page_idx = (target - data_start) / PAGE_SIZE;
            return a;
        } else {
            return NULL;
        }
    }
    return NULL;
}

static inline void *arena_page_addr(struct arena *arena, size_t idx)
{
    return (void *)((uintptr_t)arena + (idx + 1) * PAGE_SIZE);
}

#endif /* TRIMALLOC_INTERNAL_H */
