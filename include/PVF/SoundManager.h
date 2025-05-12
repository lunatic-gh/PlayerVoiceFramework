#pragma once

namespace PVF {
    struct SoundEvent {
        std::string name;
        int chance;
        int cooldown;
        std::vector<std::string> overrideBlacklist;
        std::vector<std::pair<std::string, std::vector<std::string>>> audios;
    };

    class SoundManager {
    public:
        static SoundManager* GetSingleton();

        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        void RegisterSoundEvent(const std::string& packName, const std::string& name, int chance, int cooldown, const std::vector<std::string>& overrideBlacklist, const std::vector<std::pair<std::string, std::vector<std::string>>>& audios);

        void SendSoundEvent(const std::string& name);

        void StopCurrentSoundEvent();

        bool IsSoundEventPlaying() const;

        std::optional<SoundEvent> GetCurrentSoundEvent() const;

        std::optional<RE::BSSoundHandle> GetCurrentSoundHandle() const;

        std::vector<SoundEvent> GetRegisteredSoundEvents(const std::string& pack) const;

        std::vector<std::string> GetRegisteredPacks() const;

    private:
        void SendSoundEvent(const SoundEvent& event);
        SoundManager();
        std::unordered_map<std::string, std::vector<SoundEvent>> registeredSoundEvents;
        std::optional<SoundEvent> currentSoundEvent = std::nullopt;
        std::optional<RE::BSSoundHandle> currentHandle = std::nullopt;
    };
}