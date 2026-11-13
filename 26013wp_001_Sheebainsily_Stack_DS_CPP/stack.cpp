#include "stack.hpp"

#include <iostream>
#include <new>

Stack::Stack() : top_(nullptr), size_(0) {}

Stack::~Stack() {
    clear();
}

bool Stack::push(int value) {
    Node* newNode = new (std::nothrow) Node(value, top_);
    if (newNode == nullptr) {
        return false;
    }

    top_ = newNode;
    ++size_;
    return true;
}

bool Stack::pop(int& removedValue) {
    if (isEmpty()) {
        return false;
    }

    Node* temp = top_;
    removedValue = temp->data;
    top_ = top_->next;
    delete temp;
    --size_;
    return true;
}

bool Stack::peek(int& topValue) const {
    if (isEmpty()) {
        return false;
    }

    topValue = top_->data;
    return true;
}

bool Stack::isEmpty() const {
    return top_ == nullptr;
}

std::size_t Stack::size() const {
    return size_;
}

void Stack::display() const {
    if (isEmpty()) {
        std::cout << "Stack is empty.\n";
        return;
    }

    std::cout << "Stack (top to bottom): ";
    for (Node* current = top_; current != nullptr; current = current->next) {
        std::cout << current->data;
        if (current->next != nullptr) {
            std::cout << " -> ";
        }
    }
    std::cout << '\n';
}

void Stack::clear() {
    while (top_ != nullptr) {
        Node* temp = top_;
        top_ = top_->next;
        delete temp;
    }
    size_ = 0;
}
