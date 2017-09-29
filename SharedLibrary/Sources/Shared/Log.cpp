#pragma once

#include "Log.hpp"

namespace uf {

	Log &Log::operator<<(long long int a) {
		mutex.lock();
		std::cout << a << ' ';
		mutex.unlock();
		return *this;
	}

	Log &Log::operator<<(std::string str) {
		mutex.lock();
		std::cout << str << ' ';
		mutex.unlock();
		return *this;
	}

    Log &Log::operator<<(std::wstring str) {
        mutex.lock();
        std::wcout << str << ' ';
        mutex.unlock();
        return *this;
    }

	Log &Log::operator<<(std::ostream& f(std::ostream &os)) {
		mutex.lock();
		std::cout << std::endl;
		mutex.unlock();
		return *this;
	}

}