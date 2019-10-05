#include <gtest/gtest.h>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/MessageOnlyFormatter.h>

int main(int argc, char **argv) {
	plog::ConsoleAppender<plog::MessageOnlyFormatter> appender;
	plog::init(plog::verbose, &appender);

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
