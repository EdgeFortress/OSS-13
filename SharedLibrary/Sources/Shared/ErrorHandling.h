#pragma once

#include <sstream>
#include <plog/Log.h>

struct ExpectationFailedException : public std::exception {
	ExpectationFailedException(const char *file, const char *function, int line) :
		file(file),
		function(function),
		line(line)
	{ }

	const char *what() const noexcept override {
		std::stringstream ss;
		ss << "Expectation failed\n"
			<< "\t" << "File: " << file
			<< "\t" << "Function: " << function
			<< "\t" << "Line: " << line;
		return ss.str().c_str();
	}

	const char *file;
	const char *function;
	int line;
};

#define _ASSERT_MSG(cond) LOGE << "Assertion " << #cond << " failed"

// Assert for condition. When false, write error to log and exit.
#define ASSERT(cond) if (!(cond)) { _ASSERT_MSG(cond); exit(1); }

// Assert for condition. When false, write error with specified message to log and exit.
#define ASSERT_WITH_MSG(cond, msg) if (!(cond)) { _ASSERT_MSG(cond) << ". " << (msg); exit(1); } 

#define _EXPECT_MSG(cond) LOGE << "Expectation " << #cond << " failed"

// Expect for condition. When false, write error to log and throw exception.
#define EXPECT(cond) if (!(cond)) { _EXPECT_MSG(cond); throw ExpectationFailedException(__FILE__, __func__, __LINE__); }

// Expect for condition. When false, write error with specified message to log and throw exception.
#define EXPECT_WITH_MSG(cond, msg) if (!(cond)) { _EXPECT_MSG(cond) << ", " << (msg); throw ExpectationFailedException(__FILE__, __func__, __LINE__); }

// Exception managing macroses for using in catch block
#define MANAGE_EXCEPTION(e) LOGE << __FUNCTION__ << " failed with exception:\n" << e.what()
#define MANAGE_EXCEPTION_WITH_MSG(e, msg) MANAGE_EXCEPTION((e)) << "\n" << (msg)
