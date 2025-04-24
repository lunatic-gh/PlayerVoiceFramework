#include "../include/PVE.h"

namespace PVE {
    SoundManager& SoundManager::GetSingleton() {
        static SoundManager instance;
        return instance;
    }

    void SoundManager::RegisterSoundEvent(const std::string& name, const int chance, const float cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios) {
        this->registeredSoundEvents[name] = SoundEvent{name, chance, cooldown, overrideBlacklist, audios};
    }

    bool SoundManager::PlaySoundEvent(const SoundEvent& event) {
        static std::thread cleanupThread;
        static std::atomic cancelCleanup(false);
        if (cleanupThread.joinable()) {
            cancelCleanup.store(true);
            cleanupThread.join();
        }
        if (!this->IsSoundEventPlaying() || !Util::Contains(currentSoundEvent->overrideBlacklist, event.name)) {
            for (auto [expr, files] : event.audios) {
                if (ConditionManager::GetSingleton()->EvaluateExpression(event.name, expr)) {
                    const auto filePath = std::format("Sound/PlayerVoiceEvents/SoundData/{}", files.at(Util::RandomInt(0, static_cast<int>(files.size()) - 1)));
                    RE::BSSoundHandle handle;
                    RE::BSResource::ID id;
                    id.GenerateFromPath(filePath.c_str());
                    RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(handle, id, 0x1A, 1);
                    this->currentHandle.emplace(handle);
                    this->currentSoundEvent.emplace(event);
                    this->currentHandle->Play();
                    Util::LogDebug("Played file {}", filePath);
                    cancelCleanup.store(false);
                    cleanupThread = std::thread([this]() {
                        uint64_t elapsed = 0;
                        while (!cancelCleanup.load() && !this->currentHandle->IsPlaying()) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(5));
                            elapsed += 5;
                            if (elapsed >= 1000) {
                                Util::LogWarning("Last sound could not be detected as 'playing' - This is a known issue within the Skyrim Engine.");
                                break;
                            }
                        }
                        const uint64_t duration = this->currentHandle->GetDuration();
                        uint64_t sleepTime = duration > elapsed ? duration - elapsed : 0;
                        while (sleepTime > 0 && !cancelCleanup.load()) {
                            constexpr uint64_t interval = 10;
                            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                            sleepTime = (sleepTime > interval) ? sleepTime - interval : 0;
                        }
                        this->StopCurrentSoundEvent();
                        Util::LogDebug("Done.");
                    });
                    return true;
                }
                Util::LogDebug("Condition '{}' is false", expr);
            }
        }
        return false;
    }

    bool SoundManager::PlaySoundEvent(const std::string& name) {
        if (const auto it = this->registeredSoundEvents.find(name); it != this->registeredSoundEvents.end()) {
            return PlaySoundEvent(it->second);
        }
        return false;
    }

    void SoundManager::StopCurrentSoundEvent() {
        if (this->currentHandle.has_value()) {
            this->currentHandle->Stop();
            this->currentHandle.reset();
        }
        this->currentSoundEvent.reset();
    }

    bool SoundManager::IsSoundEventPlaying() const {
        return this->currentSoundEvent.has_value();
    }
}