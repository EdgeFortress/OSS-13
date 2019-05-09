#include "Log.hpp"

namespace uf {

	Log &Log::operator<<(long long int a) {
		std::scoped_lock lock(mutex);
		std::cout << a << ' ';
		return *this;
	}

	Log &Log::operator<<(std::string str) {
		std::scoped_lock lock(mutex);
		std::cout << str << ' ';
		return *this;
	}

    Log &Log::operator<<(std::wstring str) {
		std::scoped_lock lock(mutex);
        std::wcout << str << ' ';
        return *this;
    }

	Log &Log::operator<<(std::ostream& f(std::ostream &os)) {
		std::scoped_lock lock(mutex);
		std::cout << std::endl;
		return *this;
	}

}