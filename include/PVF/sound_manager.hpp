#pragma once

#include <lntc/lunatic_utils.hpp>
#include <string>
#include <utility>

namespace pvf {
  class Sound {
  public:
    explicit Sound(const std::vector<std::tuple<std::string, std::string, std::string>>& conditions, const std::vector<std::pair<std::string, double>>& audio_files,
                   const bool force_play)
        : conditions_(conditions), audio_files_(audio_files), force_play_(force_play) {}

    [[nodiscard]] std::vector<std::tuple<std::string, std::string, std::string>> conditions() const { return conditions_; }

    [[nodiscard]] std::vector<std::pair<std::string, double>> audio_files() const { return audio_files_; }

    [[nodiscard]] bool force_play() const { return force_play_; }

  private:
    std::vector<std::tuple<std::string, std::string, std::string>> conditions_;
    std::vector<std::pair<std::string, double>> audio_files_;
    bool force_play_;
  };

  class SoundManager : public lntc::Singleton<SoundManager> {
    friend class Singleton;

  public:
    void play(const std::string& event_name /* Unused for now */, const std::string& path, const bool force_play) {
      std::lock_guard lock(this->mutex_);
      if (this->handle_.IsValid()) {
        if (!force_play) {
          log_debug("Ignoring sound...");
          return;
        }
        this->handle_.Stop();
        log_debug("Stopped previous sound...");
      }
      RE::BSResource::ID id;
      id.GenerateFromPath(path.c_str());
      RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(this->handle_, id, 0x1A, 1);
      this->handle_.Play();
      log_info("Playing sound '{}'", path);
    }

  private:
    RE::BSSoundHandle handle_;
    std::mutex mutex_;

  protected:
    SoundManager() {}
  };
}
