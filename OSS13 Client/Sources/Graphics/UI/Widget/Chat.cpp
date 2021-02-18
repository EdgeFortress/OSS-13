#include "Chat.h"

#include <fstream>
#include <sstream>

#include <Shared/inja.hpp>

Chat::Chat(uf::vec2i size, BrowserController& browserController) :
    BrowserWidget(size, browserController)
{
    std::ifstream stream("Resources/Templates/chat.tmpl");
    std::stringstream buffer;
    buffer << stream.rdbuf();

    contentTemplate = buffer.str();
    AddToContent("");
}

void Chat::AddToContent(const std::string& newContent) {
    contentMessages += newContent;

    inja::json data;
    data["content"] = contentMessages;

    SetContent(inja::render(contentTemplate, data));
}
