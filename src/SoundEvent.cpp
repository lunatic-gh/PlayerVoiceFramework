#include "SoundEvent.h"

#include "Utils.h"

namespace PVE {

    SoundEvent::SoundEvent(const int chance, const float cooldown, const bool canBeOverridden, const bool forceOverrideOthers, const float delay, const float volume,
                           const std::vector<std::string>& files) {
        this->chance = chance;
        this->cooldown = cooldown;
        this->canBeOverridden = canBeOverridden;
        this->forceOverrideOthers = forceOverrideOthers;
        this->delay = delay;
        this->volume = volume;
        this->audioFiles = files;
    }

    void SoundEvent::Play() {
        std::random_device randomDev;
        std::mt19937 randomGen(randomDev());
        std::uniform_int_distribution<> randomRange(0, static_cast<int>(this->audioFiles.size() - 1));
        const std::string file = this->audioFiles.at(randomRange(randomGen));
        RE::BSResource::ID id;
        id.GenerateFromPath(std::format("Sound/PlayerVoiceEvents/SoundData/{}", file).c_str());
        RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(handle, id, 0x1A, 1);
        handle.SetVolume(this->volume);
        handle.SetObjectToFollow(RE::PlayerCharacter::GetSingleton()->GetCurrent3D());
        handle.Play();
        Utils::Log(std::format("Played sound '{}'", file));
    }

    void SoundEvent::Stop() {
        if (handle.IsValid()) {
            handle.Stop();
        }
    }

    bool SoundEvent::IsPlaying() const {
        return handle.IsPlaying();
    }

    std::vector<std::string> SoundEvent::GetAudioFiles() {
        return this->audioFiles;
    }

    RE::BSSoundHandle SoundEvent::GetHandle() {
        return handle;
    }

    int SoundEvent::GetChance() {
        return this->chance;
    }

    float SoundEvent::GetCooldown() {
        return this->cooldown;
    }

    bool SoundEvent::CanBeOverridden() {
        return this->canBeOverridden;
    }

    bool SoundEvent::IsForceOverrideOthers() {
        return this->forceOverrideOthers;
    }

    float SoundEvent::GetDelay() {
        return delay;
    }

    float SoundEvent::GetVolume() {
        return volume;
    }
}