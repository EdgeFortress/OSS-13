#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

class Chat {
private:    


public:
    Chat();

    Chat(Chat &chat) = delete;

    ~Chat() = default;
};
