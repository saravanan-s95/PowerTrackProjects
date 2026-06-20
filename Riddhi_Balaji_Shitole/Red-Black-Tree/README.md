# Red-Black Tree Implementation in C

## What Is This Project?

This project implements a Red-Black Tree (RBT) in C. A Red-Black Tree is a self-balancing Binary Search Tree that maintains balance using node colors and tree rotations. The balancing mechanism ensures that insertion, deletion, and search operations remain efficient even when a large number of nodes are present.

The project demonstrates the implementation of Red-Black Tree properties, node insertion, deletion, tree rotations, and automatic balancing.

## Uses Of The Project

This project is useful for understanding advanced tree data structures and self-balancing algorithms used in computer science and software development.

Main uses include:

* Learning Red-Black Tree concepts
* Understanding self-balancing Binary Search Trees
* Practicing tree rotations and recoloring
* Studying efficient search and insertion algorithms
* Learning dynamic memory allocation in C
* Understanding balanced data structures used in databases and operating systems

## Features

* Insert a node into the tree
* Delete a node from the tree
* Search for a node
* Find the minimum node
* Find the maximum node
* Delete the minimum node
* Delete the maximum node
* Display the tree structure
* Automatic balancing after insertion

## Red-Black Tree Properties

The implementation follows the standard Red-Black Tree rules:

1. Every node is either Red or Black.
2. The root node is always Black.
3. Every NULL leaf node is considered Black.
4. A Red node cannot have a Red child.
5. Every path from a node to its descendant NULL nodes contains the same number of Black nodes.

These properties help maintain a balanced tree and guarantee O(log n) performance for major operations.

## Project Files

* `main.c` : Driver program and menu handling
* `insert.c` : Node insertion implementation
* `delete.c` : Node deletion implementation
* `find_min.c` : Find minimum node in the tree
* `find_max.c` : Find maximum node in the tree
* `delete_min.c` : Delete minimum node
* `delete_max.c` : Delete maximum node
* `fixup.c` : Balancing and recoloring logic
* `rotate.c` : Left and right tree rotations
* `print_tree.c` : Tree display functions
* `rbt.h` : Header file containing structure definitions and function declarations

## Requirements

You need a C compiler installed, such as GCC.

Check GCC installation:

```bash
gcc --version
```

## How To Run The Project

1. Open the project folder:

```bash
cd red-black-tree/RedBlackTree
```

2. Compile all source files:

```bash
gcc *.c -o rbt
```

3. Run the executable:

```bash
./rbt
```

## Example Operations

```text
1. Create a tree
2. Display
3. Search a node
4. Find Maximum node in Tree
5. Find Minimum node in Tree
6. Deletion
7. Delete Minimum node
8. Delete Maximum Node
9. Exit
```

Example insertion:

```text
Enter the data to be inserted into the RB Tree: 18

Now Tree is balanced

(18)--(BLACK->1)
```

## Time Complexity

| Operation    | Complexity |
| ------------ | ---------- |
| Search       | O(log n)   |
| Insert       | O(log n)   |
| Delete       | O(log n)   |
| Find Minimum | O(log n)   |
| Find Maximum | O(log n)   |

## Concepts Used

* Red-Black Trees
* Binary Search Trees
* Tree Rotations
* Recursion
* Dynamic Memory Allocation
* Data Structures in C

## Notes

* The project is implemented entirely in C.
* Automatic balancing is performed after insertion.
* Duplicate values are not allowed.
* The tree maintains Red-Black Tree properties throughout operations.


