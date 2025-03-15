#include "Utils.h"

#include <future>
#include <nlohmann/json.hpp>

namespace PVE {
    void Utils::LoadConfig() {
        const std::string path = "Data/Sound/FX/LNTC_PlayerVoiceEvents/config.json";
        std::ifstream file(path);
        if (!file.is_open()) {
            Log(std::format("Warning: Could not find file '{}' - If you don't have a Voice-Pack installed, this can be safely ignored.", path));
            return;
        }

        nlohmann::json config;
        try {
            file >> config;
        } catch (const nlohmann::json::parse_error &) {
            Log(std::format("Error: Could not parse Voice-Pack Configuration - Please make sure it is in proper json-format."));
            return;
        }

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
                soundEvent.files.push_back(f.get<std::string>());
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
        Log(std::format("Attempting to play sound for event '{}'", s));
        SoundEvent event;
        if (subEventIt != registeredSoundEvents.end() && !subEventIt->second.files.empty()) {
            event = subEventIt->second;
        } else if (eventIt != registeredSoundEvents.end() && !eventIt->second.files.empty()) {
            event = eventIt->second;
        } else {
            return;
        }
        if (!currentSound.has_value() || (currentSound.value().canBeOverridden || event.forceOverrideOthers)) {
            if (cooldownMap[soundEventName] == 0.0f) {
                if (GenerateRandomInt(0, 99) < event.chance) {
                    StopSound();
                    currentSound.emplace(event);
                    cooldownMap[soundEventName] = event.cooldown;
                    const std::string file = event.files[GenerateRandomInt(0, static_cast<int>(event.files.size()) - 1)];
                    RunConsoleCommand("player.speaksound \"FX/LNTC_PlayerVoiceEvents/" + file + "\"");
                }
            }
        }
    }
}