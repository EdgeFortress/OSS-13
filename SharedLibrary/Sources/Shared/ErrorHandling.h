#pragma once

#include <plog/Log.h>

struct ExpectationFailedException : public std::exception {
	const char *what() const noexcept override {
		return "Expectation failed";
	}
};

#define _ASSERT_MSG(cond) LOGE << "Assertion " << #cond << " failed"

// Assert for condition. When false, write error to log and exit.
#define ASSERT(cond) if (!(cond)) { _ASSERT_MSG(cond); exit(1); }

// Assert for condition. When false, write error with specified message to log and exit.
#define ASSERT_WITH_MSG(cond, msg) if (!(cond)) { _ASSERT_MSG(cond) << ". " << (msg); exit(1); } 

#define _EXPECT_MSG(cond) LOGE << "Expectation " << #cond << " failed"

// Expect for condition. When false, write error to log and throw exception.
#define EXPECT(cond) if (!(cond)) { _EXPECT_MSG(cond); throw ExpectationFailedException(); }

// Expect for condition. When false, write error with specified message to log and throw exception.
#define EXPECT_WITH_MSG(cond, msg) if (!(cond)) { _EXPECT_MSG(cond) << ", " << (msg); throw ExpectationFailedException(); }
