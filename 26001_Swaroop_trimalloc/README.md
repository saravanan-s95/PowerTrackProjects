trimalloc

a three-layer memory allocator written in C 

Architecture:

trimalloc routes allocations into three distinct layers based on the requested size:
1. Slabs (<= 2048 bytes): Small objects are grouped into 24 size classes. Each slab page serves objects of a single size class, using an embedded freelist and tracking allocations via a bitmask.
2. Page-Runs (<= 128 KB): Medium allocations are assigned contiguous 4 KB pages inside 2 MB arenas mapped from the OS.
3. Large Allocations (> 128 KB): Large allocations are mapped directly from the OS (using mmap or VirtualAlloc) and tracked in a global list.
