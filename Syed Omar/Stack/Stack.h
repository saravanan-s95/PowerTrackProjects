#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <cstddef>

// A simple templated, array-based stack (LIFO) implementation.
template <typename T>
class Stack {
private:
    T* data;            // underlying dynamic array
    size_t capacity;     // max number of elements currently allocated
    size_t topIndex;     // index of the next free slot (== current size)

    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < topIndex; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // Construct a stack with an initial capacity (default 10, grows automatically).
    explicit Stack(size_t initialCapacity = 10)
        : data(new T[initialCapacity]), capacity(initialCapacity), topIndex(0) {}

    // Rule of three: destructor, copy constructor, copy assignment.
    ~Stack() {
        delete[] data;
    }

    Stack(const Stack& other)
        : data(new T[other.capacity]), capacity(other.capacity), topIndex(other.topIndex) {
        for (size_t i = 0; i < topIndex; ++i) {
            data[i] = other.data[i];
        }
    }

    Stack& operator=(const Stack& other) {
        if (this == &other) return *this;
        T* newData = new T[other.capacity];
        for (size_t i = 0; i < other.topIndex; ++i) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        capacity = other.capacity;
        topIndex = other.topIndex;
        return *this;
    }

    // Push a value onto the top of the stack. Grows the stack if full.
    void push(const T& value) {
        if (topIndex == capacity) {
            resize(capacity == 0 ? 1 : capacity * 2);
        }
        data[topIndex++] = value;
    }

    // Remove and return the top value. Throws if the stack is empty.
    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack::pop() called on an empty stack");
        }
        return data[--topIndex];
    }

    // Return (without removing) the top value. Throws if the stack is empty.
    T& peek() {
        if (isEmpty()) {
            throw std::underflow_error("Stack::peek() called on an empty stack");
        }
        return data[topIndex - 1];
    }

    const T& peek() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack::peek() called on an empty stack");
        }
        return data[topIndex - 1];
    }

    bool isEmpty() const {
        return topIndex == 0;
    }

    size_t size() const {
        return topIndex;
    }

    void clear() {
        topIndex = 0;
    }
};

#endif // STACK_H
