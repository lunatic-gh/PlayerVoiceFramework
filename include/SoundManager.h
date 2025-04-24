#pragma once
#include <API.h>

namespace PVE {
    struct SoundEvent {
        std::string name;
        int chance;
        float cooldown;
        std::vector<std::string> overrideBlacklist;
        std::vector<std::pair<std::string, std::vector<std::string>>> audios;
    };

    class SoundManager {
    public:
        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        static SoundManager& GetSingleton();

        void RegisterSoundEvent(const std::string& name, int chance, float cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios);

        bool SendSoundEvent(const SoundEvent& event);

        bool SendSoundEvent(const std::string& name);

        void StopCurrentSoundEvent();

        bool IsSoundEventPlaying() const;

        [[nodiscard]] std::unordered_map<std::string, SoundEvent> GetRegisteredSoundEvents() const {
            return registeredSoundEvents;
        }

    private:
        SoundManager() = default;
        std::unordered_map<std::string, SoundEvent> registeredSoundEvents;
        std::optional<SoundEvent> currentSoundEvent;
        std::optional<RE::BSSoundHandle> currentHandle;
    };
}