#ifndef TRIMALLOC_H
#define TRIMALLOC_H

#include <stddef.h>

/* Initialize the allocator. Subsequent calls are no-ops. */
void trimalloc_init(void);

/* Allocate at least `size` bytes. Returns 16-byte aligned pointer, or NULL. */
void *trimalloc(size_t size);

/* Release memory. Safe to pass NULL. */
void trifree(void *ptr);

/* Allocate zero-initialized array memory. Returns NULL on overflow/failure. */
void *tricalloc(size_t nmemb, size_t size);

/* Resize allocation. Returns NULL on failure (original remains valid). */
void *trirealloc(void *ptr, size_t new_size);

#ifdef TRIMALLOC_DEBUG
/* Validate internal state. Returns 0 on success, -1 on error. */
int trimalloc_validate(void);
#endif

/* Print leaked allocations to stderr. */
void trimalloc_report_leaks(void);

#endif /* TRIMALLOC_H */
