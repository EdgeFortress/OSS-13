#pragma once

#include <queue>
#include <mutex>

namespace uf {

	template<class T>
	class ThreadSafeQueue {
	private:
		std::queue<T> queue;
		std::mutex mutex;
	public:
		void Push(T t);
		T Pop();
		bool Empty();
	};

	template<class T>
	void ThreadSafeQueue<T>::Push(T t) {
		mutex.lock();
		queue.push(t);
		mutex.unlock();
	}

	template<class T>
	T ThreadSafeQueue<T>::Pop() {
		mutex.lock();
		if (queue.empty())
			return nullptr;
		T t = queue.front();
		queue.pop();
		mutex.unlock();
		return t;
	}

	template<class T>
	bool ThreadSafeQueue<T>::Empty() {
		mutex.lock();
		bool empty = queue.empty();
		mutex.unlock();
		return empty;
	}

}