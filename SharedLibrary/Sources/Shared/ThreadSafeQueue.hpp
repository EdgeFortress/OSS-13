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
		uint32_t GetSize();
	};

	template<class T>
	void ThreadSafeQueue<T>::Push(T t) {
		std::scoped_lock lock(mutex);
		queue.push(t);
	}

	template<class T>
	T ThreadSafeQueue<T>::Pop() {
		std::scoped_lock lock(mutex);
		if (queue.empty())
			return nullptr;
		T t = queue.front();
		queue.pop();
		return t;
	}

	template<class T>
	bool ThreadSafeQueue<T>::Empty() {
		std::scoped_lock lock(mutex);
		bool empty = queue.empty();
		return empty;
	}

	template<class T>
	uint32_t ThreadSafeQueue<T>::GetSize() {
		std::scoped_lock lock(mutex);
		return static_cast<uint>(queue.size());
	}

}