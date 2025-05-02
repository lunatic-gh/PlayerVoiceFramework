#pragma once

#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>

namespace PVE {
    class Logger {
    public:
        static Logger* getInstance() {
            if (!logger_ptr) logger_ptr = new Logger("Data/SKSE/Plugins/PlayerVoiceEvents.log");
            return logger_ptr;
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        template <class... Args>
        void Log(const std::string& level, std::format_string<Args...> fmt, Args&&... args) {
            if (logFile.is_open()) {
                logFile << std::format("[{}] [{}] {}", GetTimestamp(), level, std::format(fmt, std::forward<Args>(args)...)) << std::endl;
                logFile.flush();
            }
        }

        template <class... Args>
        static void LogInfo(std::format_string<Args...> fmt, Args&&... args) {
            logger_ptr->Log("INFO", fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        static void LogWarning(std::format_string<Args...> fmt, Args&&... args) {
            logger_ptr->Log("WARNING", fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        static void LogError(std::format_string<Args...> fmt, Args&&... args) {
            logger_ptr->Log("ERROR", fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        static void LogCritical(std::format_string<Args...> fmt, Args&&... args) {
            logger_ptr->Log("CRITICAL", fmt, std::forward<Args>(args)...);
        }

        template <class... Args>
        static void LogDebug(std::format_string<Args...> fmt, Args&&... args) {
            logger_ptr->Log("DEBUG", fmt, std::forward<Args>(args)...);
        }

        std::string GetTimestamp() {
            const auto now = std::chrono::system_clock::now();
            const auto l = std::chrono::system_clock::to_time_t(now);
            const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            std::tm buf;
            localtime_s(&buf, &l);

            return std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}", buf.tm_year + 1900, buf.tm_mon + 1, buf.tm_mday, buf.tm_hour, buf.tm_min, buf.tm_sec, ms.count());
        }

        ~Logger() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

    private:
        static inline Logger* logger_ptr = nullptr;
        std::ofstream logFile;

        explicit Logger(const std::string& filePath) {
            try {
                logFile.open(filePath, std::ios::out | std::ios::trunc);
            } catch (...) {
            }
        }
    };
}