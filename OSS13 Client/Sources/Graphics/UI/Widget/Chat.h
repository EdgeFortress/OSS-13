#pragma once

#include "BrowserWidget.h"

class Chat : public BrowserWidget
{
public:
    Chat(uf::vec2i size, BrowserController& browserController);

    void AddToContent(const std::string &newContent);

private:
    std::string contentTemplate;
    std::string contentMessages;
};
