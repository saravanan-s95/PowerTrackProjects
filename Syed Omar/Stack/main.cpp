#include <iostream>
#include <string>
#include <limits>
#include "Stack.h"

void printMenu() {
    std::cout << "\n===== Stack Menu =====\n"
              << "1. Push\n"
              << "2. Pop\n"
              << "3. Peek (top)\n"
              << "4. Is Empty?\n"
              << "5. Size\n"
              << "6. Clear\n"
              << "0. Exit\n"
              << "Choice: ";
}

int main() {
    Stack<int> stack;
    int choice;

    std::cout << "C++ Stack Project (array-based, templated, auto-growing)\n";

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                std::cout << "Value to push: ";
                int value;
                std::cin >> value;
                stack.push(value);
                std::cout << value << " pushed onto the stack.\n";
                break;
            }
            case 2: {
                try {
                    int value = stack.pop();
                    std::cout << "Popped: " << value << "\n";
                } catch (const std::underflow_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 3: {
                try {
                    std::cout << "Top: " << stack.peek() << "\n";
                } catch (const std::underflow_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 4:
                std::cout << (stack.isEmpty() ? "Stack is empty.\n" : "Stack is NOT empty.\n");
                break;
            case 5:
                std::cout << "Size: " << stack.size() << "\n";
                break;
            case 6:
                stack.clear();
                std::cout << "Stack cleared.\n";
                break;
            case 0:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Unknown choice, try again.\n";
        }
    }
}
