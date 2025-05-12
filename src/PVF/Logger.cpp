#include "../include/PVF/Logger.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace PVF {
    void Logger::Initialize() {
        try {
            const auto opt = SKSE::log::log_directory();
            spdlog::set_default_logger(spdlog::basic_logger_mt("PlayerVoiceFramework", std::format("{}/PlayerVoiceFramework.log", opt.has_value() ? opt.value().string() : "Data/SKSE/Plugins"), true));
            spdlog::set_level(spdlog::level::trace);
            spdlog::flush_on(spdlog::level::trace);
        } catch (const spdlog::spdlog_ex& ex) {
            spdlog::error("Log initialization failed: {}", ex.what());
        }
    }

    void Logger::LogInfo(const std::string& message) {
        SKSE::log::info("{}", message);
    }

    void Logger::LogInfo(const char* message) {
        SKSE::log::info("{}", message);
    }

    void Logger::LogWarn(const std::string& message) {
        SKSE::log::warn("{}", message);
    }

    void Logger::LogWarn(const char* message) {
        SKSE::log::warn("{}", message);
    }

    void Logger::LogError(const std::string& message) {
        SKSE::log::error("{}", message);
    }

    void Logger::LogError(const char* message) {
        SKSE::log::error("{}", message);
    }

    void Logger::LogCritical(const std::string& message) {
        SKSE::log::critical("{}", message);
    }

    void Logger::LogCritical(const char* message) {
        SKSE::log::critical("{}", message);
    }

    void Logger::LogDebug(const std::string& message) {
        SKSE::log::debug("{}", message);
    }

    void Logger::LogDebug(const char* message) {
        SKSE::log::debug("{}", message);
    }
}