#include "Utils.h"

#include <future>
#include <nlohmann/json.hpp>

namespace PVE {
    void Utils::LoadConfig() {
        std::ifstream file("Data/Sound/FX/LNTC_PlayerVoiceEvents/config.json");
        nlohmann::json config;
        file >> config;
        std::map<std::string, SoundEvent> soundEvents;
        for (auto &[key, value] : config["sounds"].items()) {
            SoundEvent soundEvent;
            soundEvent.chance = value.contains("chance") && value.at("chance").is_number_integer() ? value.at("chance").get<int>() : 100;
            soundEvent.cooldown = value.contains("cooldown") && value.at("cooldown").is_number_float() ? value.at("cooldown").get<float>() : 0.0f;
            soundEvent.canBeOverridden = value.contains("canBeOverridden") && value.at("canBeOverridden").is_boolean() ? value.at("canBeOverridden").get<bool>() : false;
            soundEvent.forceOverrideOthers =
                value.contains("forceOverrideOthers") && value.at("forceOverrideOthers").is_boolean() ? value.at("forceOverrideOthers").get<bool>() : false;
            cooldownMap[key] = false;
            for (const auto &f : value.at("files")) {
                auto fileStr = f.get<std::string>();
                const size_t pos = fileStr.find(':');
                std::string filePath = fileStr.substr(0, pos);
                const float fileDuration = (pos != std::string::npos) ? std::stof(fileStr.substr(pos + 1)) : 0.0f;
                soundEvent.files[filePath] = fileDuration;
            }
            soundEvents[key] = soundEvent;
        }
        registeredSoundEvents = soundEvents;
    }

    void Utils::PlaySound(const std::string &soundEventName, const std::string &subSoundEventName) {
        const auto player = RE::PlayerCharacter::GetSingleton();
        // If Werewolf
        if (player->GetRace()->GetFormID() == 0xCDD84) {
            return;
        }
        // If not loaded into the world (e.g. in loading screens)
        if (!player->Is3DLoaded()) {
            return;
        }
        const auto eventIt = registeredSoundEvents.find(soundEventName);
        const auto subEventIt = registeredSoundEvents.find(subSoundEventName);
        std::string s = strcmp(subSoundEventName.c_str(), "") == 0 ? soundEventName : subSoundEventName;
        LogDebug(std::format("Attempting to play sound for event '{}'", s));
        SoundEvent event;
        std::string eventName = "";
        if (subEventIt != registeredSoundEvents.end() && !subEventIt->second.files.empty()) {
            event = subEventIt->second;
            eventName = subSoundEventName;
        } else if (eventIt != registeredSoundEvents.end() && !eventIt->second.files.empty()) {
            event = eventIt->second;
            eventName = soundEventName;
        } else {
            return;
        }
        if ((!prevEvent.has_value()) || (prevEvent.has_value() && prevEvent.value().canBeOverridden || event.forceOverrideOthers)) {
            RE::PlayerCharacter::GetSingleton()->GetActorRuntimeData().voiceTimer;
            if (!cooldownMap[soundEventName]) {
                const int chance = event.chance;
                float cooldown = event.cooldown;
                auto &files = event.files;
                if (generateRandomInt(0, 99) < chance) {
                    auto it = files.begin();
                    std::advance(it, generateRandomInt(0, static_cast<int>(files.size() - 1)));
                    auto &[filePath, fileDuration] = *it;
                    prevEvent.emplace(event);
                    cooldownMap[soundEventName] = true;
                    StopSound();
                    RunConsoleCommand("player.speaksound \"FX/LNTC_PlayerVoiceEvents/" + filePath + "\"");
                    std::thread([fileDuration, cooldown, soundEventName]() {
                        float f = fileDuration;
                        while (f > 0) {
                            if (!RE::UI::GetSingleton()->GameIsPaused()) f -= 0.010f;
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        }
                        prevEvent.reset();
                        f = cooldown - fileDuration;
                        while (f > 0) {
                            if (!RE::UI::GetSingleton()->GameIsPaused()) f -= 0.010f;
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        }
                        cooldownMap[soundEventName] = false;
                    }).detach();
                }
            }
        }
    }
}  // namespace PVE
