#include "SoundEvent.h"

#include "ConditionManager.h"
#include "Utils.h"

namespace PVE {

    SoundEvent::SoundEvent(const int chance, const float cooldown, const bool canBeOverridden, const bool forceOverrideOthers, const float delay, const float volume,
                           const std::vector<std::pair<std::string, std::vector<std::string>>>& files) {
        this->chance = chance;
        this->cooldown = cooldown;
        this->canBeOverridden = canBeOverridden;
        this->forceOverrideOthers = forceOverrideOthers;
        this->delay = delay;
        this->volume = volume;
        this->audios = files;
    }

    bool SoundEvent::Play() {
        for (auto [condition, files] : this->audios) {
            if (!ConditionManager::EvaluateConditions(condition)) {
                continue;
            }
            std::random_device randomDev;
            std::mt19937 randomGen(randomDev());
            std::uniform_int_distribution<> randomRange(0, static_cast<int>(files.size() - 1));
            const std::string file = files.at(randomRange(randomGen));
            RE::BSResource::ID id;
            id.GenerateFromPath(std::format("Sound/PlayerVoiceEvents/SoundData/{}", file).c_str());
            RE::BSAudioManager::GetSingleton()->BuildSoundDataFromFile(handle, id, 0x1A, 1);
            handle.SetVolume(this->volume);
            handle.SetObjectToFollow(RE::PlayerCharacter::GetSingleton()->GetCurrent3D());
            handle.Play();
            Utils::Log(std::format("Played sound '{}'", file));
            return true;
        }
        return false;
    }

    void SoundEvent::Stop() {
        if (handle.IsValid()) {
            handle.Stop();
        }
    }

    bool SoundEvent::IsPlaying() const {
        return handle.IsPlaying();
    }

    std::vector<std::pair<std::string, std::vector<std::string>>> SoundEvent::GetAudioFiles() {
        return this->audios;
    }

    RE::BSSoundHandle SoundEvent::GetHandle() {
        return this->handle;
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
        return this->delay;
    }

    float SoundEvent::GetVolume() {
        return this->volume;
    }

    // std::string SoundEvent::GetCondition() {
    //     return this->condition;
    // }
}