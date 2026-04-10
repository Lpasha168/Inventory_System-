#ifdef _WIN32
#include <windows.h>
#endif

#include "server.h"

int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Server server;
    server.start(8080);

    return 0;
}