# Stack Data Structure in C++

## 1. Project overview
This project implements a stack using a singly linked list in C++. A stack follows **LIFO (Last In, First Out)**: the most recently pushed element is removed first.

## 2. Features
- Push an integer onto the stack
- Pop the top integer
- Peek at the top element without removing it
- Display all elements from top to bottom
- Check whether the stack is empty
- Display the current stack size
- Clear the stack
- Handle pop/peek on an empty stack
- Release allocated memory safely

## 3. Files
- `main.cpp` — menu-driven user interface
- `stack.hpp` — Stack class declaration
- `stack.cpp` — stack operation implementations
- `Makefile` — build, run, and clean commands

## 4. Requirements
- Linux or Ubuntu/WSL
- GNU C++ compiler (`g++`)
- `make`

Install build tools on Ubuntu if required:
```bash
sudo apt update
sudo apt install build-essential
```

## 5. Compile and run
Open a terminal in this project directory:
```bash
make
./stack_ds
```

Or compile and run in one step:
```bash
make run
```

Remove generated object files and executable:
```bash
make clean
```

## 6. Sample run
```text
===== STACK USING LINKED LIST (C++) =====
1. Push
2. Pop
3. Peek
4. Display stack
5. Check if empty
6. Stack size
7. Clear stack
0. Exit
Enter choice: 1
Enter value to push: 10
10 pushed onto the stack.

Enter choice: 1
Enter value to push: 20
20 pushed onto the stack.

Enter choice: 4
Stack (top to bottom): 20 -> 10

Enter choice: 3
Top element: 20

Enter choice: 2
Popped value: 20

Enter choice: 6
Stack size: 1
```

## 7. Algorithm
### Push(value)
1. Allocate a new node containing `value`.
2. If allocation fails, report failure.
3. Set the new node's `next` pointer to the current top.
4. Update `top` to the new node and increment size.

### Pop()
1. If the stack is empty, report underflow.
2. Save the top node's value.
3. Move `top` to the next node.
4. Delete the old top node and decrement size.
5. Return the removed value.

### Peek()
1. If the stack is empty, report that no top element exists.
2. Otherwise, return the value stored at `top` without removing it.

## 8. Complexity analysis
| Operation | Time complexity |
|---|---|
| Push | O(1) |
| Pop | O(1) |
| Peek | O(1) |
| Is empty | O(1) |
| Size | O(1) |
| Display | O(n) |
| Clear | O(n) |

## 9. Test checklist
- [ ] Pop from an empty stack — should show underflow
- [ ] Peek at an empty stack — should report no top element
- [ ] Push one element and peek — should show that element
- [ ] Push multiple elements and display — newest element should appear first
- [ ] Pop — should remove only the top element
- [ ] Clear — should leave an empty stack with size zero
- [ ] Exit — should terminate normally

## 10. Concepts demonstrated
Classes and objects, pointers, dynamic memory allocation, linked lists, encapsulation, constructors, destructors, and stack LIFO behavior.
