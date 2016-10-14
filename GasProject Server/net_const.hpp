#pragma once

#include <memory>

template<class T>
using uptr = std::unique_ptr<T>;

enum Comand_code {
	AUTH_CODE = 1,
	REG_CODE = 2
};

enum Result {
	OK = 0, 
	AUTH_ERROR = 1, 
	REG_ERROR = 2,
	CONNECTION_ERROR = 3, 
	COMMAND_CODE_ERROR = 4
};

const int PORT = 55678;