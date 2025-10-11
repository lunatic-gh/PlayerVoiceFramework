#pragma once

#include <lntc/lunatic_utils.hpp>
#include <yaml-cpp/yaml.h>

namespace pvf {
  class Configuration : public lntc::Singleton<Configuration> {
  public:
    bool load() {
      this->node_.reset();
      try {
        if (const auto node = YAML::LoadFile("Data/SKSE/Plugins/PlayerVoiceFramework.yml"); node && node.IsMap()) {
          this->node_ = node;
          return true;
        }
      } catch (const std::exception& e) {
        log_error("{}", e.what());
      }
      this->node_ = YAML::Node(YAML::NodeType::Map);
      return false;
    }

    bool save() {
      try {
        std::ofstream out("Data/SKSE/Plugins/PlayerVoiceFramework.yml");
        out << YAML::Dump(this->node_);
        return true;
      } catch (const std::exception& e) {
        log_error("{}", e.what());
        return false;
      }
    }

    template <class T>
    void set(const char* key, const T& value) {
      this->node_[key] = value;
    }

    template <class T>
    [[nodiscard]] T get(const char* key, const T& default_value = {}) {
      try {
        return this->node_[key] ? this->node_[key].as<T>() : default_value;
      } catch (const std::exception& e) {
        log_error("{}", e.what());
      }
      return default_value;
    }

    bool has(const char* key) { return static_cast<bool>(this->node_[key]); }

    std::string dump() { return YAML::Dump(this->node_); }

  private:
    YAML::Node node_ = YAML::Node(YAML::NodeType::Map);
  };
}