# Red-Black Tree in C

## Overview

A menu-driven Red-Black Tree implementation in C that demonstrates how a self-balancing Binary Search Tree maintains efficient operations through recoloring and tree rotations.

The project implements both insertion and deletion balancing, including handling of the double-black condition during deletion. It also provides operations to find and delete the minimum and maximum elements and display the tree through inorder traversal.

## Features

* Insert elements with duplicate-value handling
* Automatic balancing after insertion
* Delete a specific element
* Handle Red-Black Tree deletion and double-black cases
* Find minimum and maximum elements
* Delete minimum and maximum elements
* LL and RR rotations with support for LR and RL cases
* Inorder traversal with node color information
* Dynamic memory allocation and deallocation
* Menu-driven user interaction

## Quick Start

Compile all source files using:

`make`

Run the program using:

`./a.out`

Once started, use the menu to choose an operation:

1. Create a tree by inserting elements
2. Display the tree
3. Delete a specific element
4. Find the minimum element
5. Delete the minimum element
6. Find the maximum element
7. Delete the maximum element
8. Exit

After an operation, entering `y` or `Y` allows the same operation to be performed again.

## Project Structure

The implementation is organized into separate modules for insertion, insertion balancing, deletion, deletion balancing, rotations, minimum/maximum operations, tree display, and the main driver program.

## Red-Black Tree Operations

Insertion and deletion maintain the Red-Black Tree properties using recoloring and rotations. Deletion handles nodes with zero, one, or two children and performs the required balancing when removal of a black node creates a double-black condition.

The minimum and maximum operations provide access to the smallest and largest elements in the tree, along with dedicated operations to remove them.

## Memory Management

Tree nodes are dynamically allocated during insertion and released when nodes are deleted. Pointers are used to maintain relationships between parent and child nodes.

## Complexity

Insertion, deletion, minimum, maximum, and search operations have O(log n) time complexity for a balanced Red-Black Tree. Traversal requires O(n) time, with O(n) memory required for the tree.

## Learning Outcomes

This project provides practical experience with Red-Black Tree algorithms, pointers, dynamic memory management, recursion, tree rotations, modular C programming, and balancing techniques.
