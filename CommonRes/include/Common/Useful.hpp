#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
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

    T Pop() {
        mutex.lock();
		if (queue.empty())
			return nullptr;
        T t = queue.front(); 
		queue.pop();
        mutex.unlock();
        return t;
    }

    bool Empty() {
        mutex.lock();
        bool empty = queue.empty();
        mutex.unlock();
        return empty;
    }
};

using std::endl;

class Log {
private:
	std::mutex mutex;
	explicit Log(const Log& log);
	Log& operator=(const Log&);

public:
	explicit Log() {}

	Log& operator<<(long long int a) {
		mutex.lock();
		std::cout << a << ' ';
		mutex.unlock();
		return *this;
	}

	Log& operator<<(std::string str) {
		mutex.lock();
		std::cout << str << ' ';
		mutex.unlock();
		return *this;
	}

	Log& operator<<(std::ostream& f(std::ostream &os)) {
		mutex.lock();
		std::cout << std::endl;
		mutex.unlock();
		return *this;
	}
};