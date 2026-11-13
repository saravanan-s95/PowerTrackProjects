#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>

class Stack {
private:
    struct Node {
        int data;
        Node* next;
        explicit Node(int value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}
    };

    Node* top_;
    std::size_t size_;

public:
    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool push(int value);
    bool pop(int& removedValue);
    bool peek(int& topValue) const;
    bool isEmpty() const;
    std::size_t size() const;
    void display() const;
    void clear();
};

#endif
