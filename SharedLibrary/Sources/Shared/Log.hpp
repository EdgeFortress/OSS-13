#pragma once

#include <mutex>
#include <iostream>
#include <string>

namespace uf {

	class Log {
	private:
		std::mutex mutex;
		//explicit Log(const Log& log);
		//Log& operator=(const Log&);

	public:
		Log& operator<<(long long int a);
		Log& operator<<(std::string str);
		Log& operator<<(std::ostream& f(std::ostream &os));
	};

}