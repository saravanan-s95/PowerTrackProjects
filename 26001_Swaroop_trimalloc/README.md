# trimalloc

a **three-layer** memory allocator written in C 

## Architecture:

trimalloc routes allocations into three distinct layers based on the requested size:
1. Slabs (<= 2048 bytes): Small objects are grouped into 24 size classes. Each slab page serves objects of a single size class, using an embedded freelist and tracking allocations via a bitmask.
2. Page-Runs (<= 128 KB): Medium allocations are assigned contiguous 4 KB pages inside 2 MB arenas mapped from the OS.
3. Large Allocations (> 128 KB): Large allocations are mapped directly from the OS (using mmap or VirtualAlloc) and tracked in a global list.

### Windows 
Build as a shared library (.dll):
> gcc -shared -o trimalloc.dll trimalloc.c slab.c page_alloc.c large.c sys_windows.c

### Linux 
Build as a shared library (.so):
> gcc -fPIC -shared -o libtrimalloc.so trimalloc.c slab.c page_alloc.c large.c sys_linux.c

### Usage 
1. Include 'trimalloc.h' in your project and link against the compiled library
2. Compilation with Shared Library:
   > Windows: gcc test.c trimalloc.dll -o test.exe
   >
   > Linux: gcc test.c ./trimalloc.so -o test

