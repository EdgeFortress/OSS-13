#pragma once

#include <memory>
#include <mutex>
#include <queue>

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
public:
    void Push(T t) {
        mutex.lock();
        queue.push(t);
        mutex.unlock();
    }

    T Front() {
        mutex.lock();
        T t = queue.front();
        mutex.unlock();
        return t;
    }

    void Pop() {
        mutex.lock();
        queue.pop();
        mutex.unlock();
    }

    bool Empty() {
        mutex.lock();
        bool empty = queue.empty();
        mutex.unlock();
        return empty;
    }
};