# Red-Black Tree in C

A menu-driven Red-Black Tree implementation in C.

## Features

- Insert nodes (duplicate keys rejected)
- Delete nodes
- Search for keys
- Find and delete minimum values
- Find and delete maximum values
- Display keys in sorted order with node colors
- Validate Red-Black Tree properties
- Dynamic memory allocation and cleanup

## Build

```sh
make
```

Alternatively, compile directly:

```sh
gcc -Wall -Wextra -Werror -std=c11 -pedantic main.c rbt.c insert.c delete.c search.c display.c -o rbt
```

## Run

```sh
./rbt
```

On Windows, use a GCC environment such as MinGW/MSYS2 or WSL. If compiled natively on Windows, run `rbt.exe`.

## Implementation Details

The public operations are declared in `rbt.h`. Core algorithms and helper functions are organized across the source files.

The tree uses a shared black NIL sentinel. The validator checks binary search tree ordering, root color, the red-parent rule, equal black height, and parent links.

## Author

**Sheeba Insily R**

**Batch Number:** 26013
