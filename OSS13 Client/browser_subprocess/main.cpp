#include <cef_app.h>

int main(int argc, char* argv[]) {
    CefMainArgs main_args;
    return CefExecuteProcess(main_args, nullptr, nullptr);
}
