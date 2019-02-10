#pragma once

#include <plog/Log.h>

namespace plog
{

template<class Formatter>
class LogRecordsHolder : public IAppender
{
public:
	virtual void write(const Record& record);

	std::vector<util::nstring> &GetMessages() { return messages; }

private:
	std::vector<util::nstring> messages;
};


template<class Formatter>
void LogRecordsHolder<Formatter>::write(const Record& record) {
	util::nstring str = Formatter::format(record);
	messages.push_back(str);
}

} // namespace plog
