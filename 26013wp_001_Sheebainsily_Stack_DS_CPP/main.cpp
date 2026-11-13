/*
Name        : Sheeba Insily R
Project     : Stack Data Structure in C++
Description : This project implements a stack using a linked list.
              It supports push, pop, peek, display and other stack operations.
              It follows the LIFO (Last In, First Out) principle.
*/

#include "stack.hpp"

#include <iostream>
#include <limits>

namespace {
void showMenu() {
    std::cout << "\n===== STACK USING LINKED LIST (C++) =====\n"
              << "1. Push\n"
              << "2. Pop\n"
              << "3. Peek\n"
              << "4. Display stack\n"
              << "5. Check if empty\n"
              << "6. Stack size\n"
              << "7. Clear stack\n"
              << "0. Exit\n"
              << "Enter choice: ";
}

bool readInt(const char* prompt, int& value) {
    std::cout << prompt;
    if (std::cin >> value) {
        return true;
    }

    std::cout << "Invalid input. Please enter an integer.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}
}

int main() {
    Stack stack;
    int choice = -1;

    while (true) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a menu number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            std::cout << "Exiting stack program. Goodbye!\n";
            break;
        }

        int value = 0;
        switch (choice) {
        case 1:
            if (!readInt("Enter value to push: ", value)) {
                break;
            }
            if (stack.push(value)) {
                std::cout << value << " pushed onto the stack.\n";
            } else {
                std::cout << "Memory allocation failed; value was not pushed.\n";
            }
            break;

        case 2:
            if (stack.pop(value)) {
                std::cout << "Popped value: " << value << '\n';
            } else {
                std::cout << "Stack underflow: cannot pop from an empty stack.\n";
            }
            break;

        case 3:
            if (stack.peek(value)) {
                std::cout << "Top element: " << value << '\n';
            } else {
                std::cout << "Stack is empty; no top element.\n";
            }
            break;

        case 4:
            stack.display();
            break;

        case 5:
            std::cout << (stack.isEmpty() ? "Stack is empty.\n" : "Stack is not empty.\n");
            break;

        case 6:
            std::cout << "Stack size: " << stack.size() << '\n';
            break;

        case 7:
            stack.clear();
            std::cout << "Stack cleared.\n";
            break;

        default:
            std::cout << "Invalid choice. Please select a listed option.\n";
            break;
        }
    }

    return 0;
}
