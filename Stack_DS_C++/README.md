# Stack Data Structure in C++

This project implements stack operations and uses a stack to convert and evaluate infix expressions into prefix and postfix expressions. It also includes a stack implementation using a linked list.

## 1. Infix to Postfix Conversion and Evaluation

**How to run:**

```bash
g++ *.cpp
./a.out
```

**Example 1**

Input:
```text
2*3-3+8/4/(1+1)
```

Expected output:
```text
PostFix expression : 2 3 * 3 - 8 4 / 1 1 + / +

Result : 4
```

**Example 2**

Input:
```text
2*30-3+8/4/(10+1)
```

Expected output:
```text
PostFix expression : 2 30 * 3 - 8 4 / 10 1 + / +

Result : 57
```

## 2. Infix to Prefix Conversion and Evaluation

**How to run:**

```bash
g++ *.cpp
./a.out
```

**Example 1**

Input:
```text
2*3-3+8/4/(1+1)
```

Expected output:
```text
PreFix expression : + - * 2 3 3 / / 8 4 + 1 1

Result : 4
```

**Example 2**

Input:
```text
2*30-3+8/4/(10+1)
```

Expected output:
```text
PreFix expression : + - * 2 30 3 / / 8 4 + 10 1

Result : 57
```

Both examples use expressions without spaces in the input. The program handles multi-digit operands, operator precedence, and parentheses. Division uses integer arithmetic.

## 3. Stack Implementation Using a Linked List

This implementation supports push, pop, display, and peek operations.

**How to run:**

```bash
g++ *.cpp
./a.out
```

**Example interaction:**

```text
1. Push
2. Pop
3. Display Stack
4. Peek(Element at Top)
5. Exit
Enter choice: 1
Enter the element you have to insert at the first : 10
Enter choice: 1
Enter the element you have to insert at the first : 20
Enter choice: 1
Enter the element you have to insert at the first : 30
Enter choice: 3
30-> 20-> 10-> NULL
Enter choice: 4
INFO : Peek element is 30
Enter choice: 2
INFO : Pop operation is successfull
Enter choice: 3
20-> 10-> NULL
Enter choice: 5
```

## Concepts Covered

- Stack operations: push, pop, peek, and display
- Stack implementation using an array and a linked list
- Infix-to-prefix and infix-to-postfix conversion
- Prefix and postfix expression evaluation
- Operator precedence and parentheses
- Handling multi-digit operands

