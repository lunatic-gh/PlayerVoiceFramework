#pragma once

#include "configuration.hpp"

#include <lntc/lunatic_utils.hpp>
#include <ranges>
#include <yaml-cpp/yaml.h>
#include <pvf/util.hpp>

namespace pvf {
  class PackManager : public lntc::Singleton<PackManager> {
  public:
    bool load() {
      const fs::path packs_dir = "Data/Sound/PlayerVoiceFramework/packs";
      if (!fs::exists(packs_dir)) {
        log_error("Could not find directory for sound-packs. This should not happen!");
        return false;
      }
      for (const fs::path pack_dir : fs::directory_iterator(packs_dir)) {
        if (!fs::is_directory(pack_dir)) continue;
        auto pack_name = pack_dir.filename().string();
        log_info("Loading Sound-Pack '{}'...", pack_name);
        fs::path config_file = pack_dir / "config.yml";
        if (!fs::exists(config_file)) {
          log_warn("  Sound-Pack '{}' has no config, skipping...", pack_name);
          continue;
        }
        auto root_node = YAML::LoadFile(config_file.string());
        std::unordered_map<std::string, std::vector<Sound>> events;
        if (const YAML::Node events_node = root_node["events"]; events_node && events_node.IsSequence()) {
          for (const YAML::Node event_node : events_node) {
            const std::string event_name = event_node["name"].as<std::string>("");
            const std::vector<std::tuple<std::string, std::string, std::string>> conditions = [&] {
              std::vector<std::tuple<std::string, std::string, std::string>> out;
              if (const YAML::Node conditions_node = event_node["conditions"]; conditions_node && conditions_node.IsSequence()) {
                for (auto condition_node : conditions_node) {
                  const std::string var = condition_node["variable"].as<std::string>("");
                  const std::string op = condition_node["operator"].as<std::string>("");
                  const std::string value = condition_node["value"].as<std::string>("");
                  if (var.empty() || op.empty() || value.empty()) {
                    continue;
                  }
                  out.emplace_back(var, op, value);
                }
              }
              return out;
            }();
            const bool force_play = event_node["force_play"].as<bool>(false);
            std::vector<std::string> audio_paths = event_node["audio_files"].as<std::vector<std::string>>(std::vector<std::string>());
            if (event_name.empty()) {
              log_info("  Found event block with empty name, skipping...");
              continue;
            }
            if (audio_paths.empty()) {
              log_info("  Found event block for '{}' without any audio files, skipping...", event_name);
              continue;
            }
            std::vector<std::pair<std::string, double>> audio_files;
            for (const std::string& rel_path : audio_paths) {
              std::string path_str = (pack_dir / rel_path).relative_path().lexically_normal().string();
              if (!fs::is_regular_file(path_str)) {
                log_info("  Found file path '{}' but it is not a valid file, skipping...", path_str);
                continue;
              }
              double duration = get_wav_duration(path_str);
              path_str = path_str.substr(path_str.find_first_of("Data/") + 5);
              lntc::str_replace_all(path_str, "\\", "/");
              audio_files.emplace_back(path_str, duration);
            }
            events[event_name].emplace_back(conditions, audio_files, force_play);
          }
        }
        this->pack_data_[pack_name] = events;
        log_info("  Loaded Sound-Pack '{}' with {} event{}.", pack_name, events.size(), events.size() == 1 ? "" : "s");
      }
      this->active_pack_ = Configuration::get_singleton()->get<std::string>("active_pack", "");
      return true;
    }

    void set_active_pack(const std::string& pack) {
      this->active_pack_ = pack;
      Configuration::get_singleton()->set("active_pack", pack);
      log_info("Active pack set to '{}'", pack);
      if (!Configuration::get_singleton()->save()) {
        log_error("Failed to save active pack to configuration.", pack);
        return;
      }
      log_info("Active pack saved to configuration.");
    }

    [[nodiscard]] std::string active_pack() { return this->active_pack_; }

    [[nodiscard]] std::vector<std::string> packs() { return std::ranges::to<std::vector>(this->pack_data_ | std::views::keys); }

    [[nodiscard]] std::unordered_map<std::string, std::vector<Sound>> sound_events_for_active_pack() {
      return this->pack_data_.contains(this->active_pack_) ? this->pack_data_.at(this->active_pack_) : std::unordered_map<std::string, std::vector<Sound>>{};
    }

  private:
    std::string active_pack_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Sound>>> pack_data_;
  };
}