#include <stdio.h>
#include <string.h>
#include "trimalloc.h"

int main() {
    printf("--- Trimalloc Basic Test ---\n");
    
    trimalloc_init();

    /* 1. Basic allocation */
    printf("\n[1] Testing trimalloc & trifree...\n");
    char *str1 = (char *)trimalloc(50);
    strcpy(str1, "Hello from trimalloc!");
    printf("Allocated string: %s\n", str1);
    trifree(str1);

    /* 2. Calloc (zeroed memory) */
    printf("\n[2] Testing tricalloc...\n");
    int *arr = (int *)tricalloc(5, sizeof(int));
    printf("Array values: %d, %d, %d, %d, %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
    
    /* 3. Realloc */
    printf("\n[3] Testing trirealloc...\n");
    arr = (int *)trirealloc(arr, 10 * sizeof(int));
    arr[5] = 42;
    printf("Reallocated array value at index 5: %d\n", arr[5]);
    trifree(arr);

    /* 4. Memory leak */
    printf("\n[4] Creating a memory leak...\n");
    char *leak = (char *)trimalloc(100);
    strcpy(leak, "Leaked string!");
    printf("Allocated 100 bytes at %p\n", (void*)leak);

    printf("\n--- Generating Leak Report ---\n");
    trimalloc_report_leaks();

    /* 5. Double free */
    printf("\n[5] Triggering a double free...\n");
    char *danger = (char *)trimalloc(20);
    trifree(danger);
    printf("Freed once...\n");
    
    /* Trigger double free crash */
    trifree(danger); 
    printf("Freed twice!\n");

    return 0;
}
