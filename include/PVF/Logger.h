#pragma once

namespace PVF {
    class Logger {
    public:
        static void Initialize();

        static void LogInfo(const std::string& message);

        static void LogWarn(const std::string& message);

        static void LogError(const std::string& message);

        static void LogCritical(const std::string& message);

        static void LogDebug(const std::string& message);
    };
}
