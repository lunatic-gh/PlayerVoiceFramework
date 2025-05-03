#pragma once

#include <spdlog/sinks/basic_file_sink.h>

namespace PVE {
    class Logger {
    public:
        static Logger& GetSingleton() {
            static Logger singleton;
            return singleton;
        }

        void LogInfo(const std::string& message) {
            spdlog::info(message);
        }

        void LogInfo(const char* message) {
            spdlog::info(message);
        }

        void LogWarn(const std::string& message) {
            spdlog::warn(message);
        }

        void LogWarn(const char* message) {
            spdlog::warn(message);
        }

        void LogError(const std::string& message) {
            spdlog::error(message);
        }

        void LogError(const char* message) {
            spdlog::error(message);
        }

        void LogCritical(const std::string& message) {
            spdlog::critical(message);
        }

        void LogCritical(const char* message) {
            spdlog::critical(message);
        }

        void LogDebug(const std::string& message) {
            spdlog::debug(message);
        }

        void LogDebug(const char* message) {
            spdlog::debug(message);
        }

    private:
        Logger() {
            try {
                const auto opt = SKSE::log::log_directory();
                spdlog::set_default_logger(spdlog::basic_logger_mt("PlayerVoiceEvents", std::format("{}/PlayerVoiceEvents.log", opt.has_value() ? opt.value().string() : "Data/SKSE/Plugins"), true));
                spdlog::set_level(spdlog::level::trace);
                spdlog::flush_on(spdlog::level::trace);
            } catch (const spdlog::spdlog_ex& ex) {
                spdlog::error("Log initialization failed: {}", ex.what());
            }
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    };
}
