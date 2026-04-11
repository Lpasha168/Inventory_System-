#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <ctime>

class Logger {
public:
    static void log(const std::string& message) {
        std::time_t now = std::time(nullptr);

        char* dt = std::ctime(&now);
        dt[strlen(dt) - 1] = '\0'; 

        std::cout << "[" << dt << "] " << message << std::endl;
    }
};

#endif
