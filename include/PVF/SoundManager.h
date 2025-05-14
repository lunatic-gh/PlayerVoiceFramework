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

        std::vector<SoundEvent> GetRegisteredSoundEvents(const std::string& pack) const;

        std::vector<std::string> GetRegisteredPacks() const;

    private:
        SoundManager();

        void SendSoundEvent(const SoundEvent& event, const std::string& pack);

        static std::string GetRandomFile(const std::string& pack, std::vector<std::string> files);

        void StopCurrentSoundEvent();

        std::unordered_map<std::string, std::vector<SoundEvent>> registeredSoundEvents;
        std::optional<SoundEvent> currentSoundEvent = std::nullopt;
        std::optional<RE::BSSoundHandle> currentSoundHandle = std::nullopt;
        std::atomic<unsigned int> currentSoundID{0};
    };
}