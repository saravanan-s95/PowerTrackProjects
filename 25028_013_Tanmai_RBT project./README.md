# Red Black Tree

## Description

A Red Black Tree is a self-balancing Binary Search Tree that maintains balance using node colors and tree rotations.

This project implements the Red Black Tree operations in C.

## Features

- Insert a node
- Delete a node
- Search for a node
- Find minimum value
- Find maximum value
- Delete minimum value
- Delete maximum value
- Display the tree using inorder traversal
- Red Black Tree rotations and rebalancing

## Technologies Used

- C Programming
- Data Structures
- Red Black Tree
- Dynamic Memory Allocation
- Pointers
- Structures

## Project Files

| File | Description |
|---|---|
| `main.c` | Menu-driven main program |
| `rbt.h` | Structure definitions and function prototypes |
| `insert.c` | Red Black Tree insertion |
| `delete.c` | Red Black Tree deletion |
| `search.c` | Search operation |
| `find_min.c` | Find minimum element |
| `find_max.c` | Find maximum element |
| `delete_min.c` | Delete minimum element |
| `delete_max.c` | Delete maximum element |
| `print_tree.c` | Inorder tree display |

## Compilation

Compile the project using:

```bash
gcc -Wall -Wextra main.c insert.c delete.c search.c delete_min.c delete_max.c find_min.c find_max.c print_tree.c -o rbt
