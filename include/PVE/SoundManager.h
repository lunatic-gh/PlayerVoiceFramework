#pragma once

namespace PVE {
    struct SoundEvent {
        std::string name;
        int chance;
        int cooldown;
        std::vector<std::string> overrideBlacklist;
        std::vector<std::pair<std::string, std::vector<std::string>>> audios;
    };

    class SoundManager {
    public:
        static SoundManager* GetSingleton() {
            static SoundManager instance;
            return &instance;
        }

        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        void RegisterSoundEvent(const std::string& name, int chance, int cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios);

        void SendSoundEvent(const SoundEvent& event);

        void SendSoundEvent(const std::string& name);

        void StopCurrentSoundEvent();

        bool IsSoundEventPlaying() const;

        [[nodiscard]] std::optional<SoundEvent> GetCurrentSoundEvent() const {
            return currentSoundEvent;
        }
        [[nodiscard]] std::optional<RE::BSSoundHandle> GetCurrentSoundHandle() const {
            return currentHandle;
        }

        [[nodiscard]] std::unordered_map<std::string, SoundEvent> GetRegisteredSoundEvents() const {
            return registeredSoundEvents;
        }

    private:
        SoundManager();
        std::unordered_map<std::string, SoundEvent> registeredSoundEvents;
        std::optional<SoundEvent> currentSoundEvent = std::nullopt;
        std::optional<RE::BSSoundHandle> currentHandle = std::nullopt;
    };
}