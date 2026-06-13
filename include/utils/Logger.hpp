#pragma once
// Placeholder for Logger

#include <iostream>
#include <string>

namespace brainrot::utils
{

    class Logger
    {
    public:
        static void info(const std::string &message)
        {
            std::clog << "[INFO] " << message << '\n';
        }

        static void warn(const std::string &message)
        {
            std::clog << "[WARN] " << message << '\n';
        }

        static void error(const std::string &message)
        {
            std::cerr << "[ERROR] " << message << '\n';
        }
    };

} // namespace brainrot::utils