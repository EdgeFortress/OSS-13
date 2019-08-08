#pragma once

#include <mutex>
#include <plog/Log.h>

namespace plog
{

template<class Formatter>
class LogRecordsHolder : public IAppender
{
public:
	virtual void write(const Record& record);

	std::vector<util::nstring> GetMessages() { return std::move(messages); }
	void Lock() { mutex.lock(); } // TODO: refact this
	void Unlock() { mutex.unlock(); }

private:
	std::vector<util::nstring> messages;
	std::mutex mutex;
};


template<class Formatter>
void LogRecordsHolder<Formatter>::write(const Record& record) {
	std::unique_lock<std::mutex> lock(mutex);
	util::nstring str = Formatter::format(record);
	messages.push_back(str);
}

} // namespace plog
