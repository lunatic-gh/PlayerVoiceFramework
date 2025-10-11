#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace pvf {
  inline void init_logger() {
    const auto log_dir = SKSE::log::log_directory();
    if (!log_dir.has_value()) {
      return;
    }
    const auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>((log_dir.value() / "PlayerVoiceFramework.log").string(), true);
    const auto logger = std::make_shared<spdlog::logger>("PlayerVoiceFramework", std::move(sink));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");
    spdlog::set_default_logger(std::move(logger));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
  }

#define log_info(fmt, ...) spdlog::info(fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) spdlog::warn(fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) spdlog::error(fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) spdlog::debug(fmt, ##__VA_ARGS__)
}