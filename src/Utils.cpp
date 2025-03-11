#include "Utils.h"

#include <future>
#include <unordered_set>
#include <nlohmann/json.hpp>

namespace PVE {
    void Utils::LoadConfig() {
        std::ifstream file("Data/Sound/FX/LNTC_PlayerVoiceEvents/config.json");
        nlohmann::json config;
        file >> config;
        std::map<std::string, SoundEvent> soundEvents;
        for (auto &[key, value]: config["sounds"].items()) {
            SoundEvent soundEvent;
            soundEvent.chance = value.contains("chance") && value.at("chance").is_number_integer() ? value.at("chance").get<int>() : 100;
            soundEvent.cooldown = value.contains("cooldown") && value.at("cooldown").is_number_float() ? value.at("cooldown").get<float>() : 0.0f;
            soundEvent.overrideMode = value.contains("overrideMode") && value.at("overrideMode").is_number_integer() ? value.at("overrideMode").get<int>() : 0;
            for (const auto &f: value.at("files")) {
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
        auto categoriesMap = registeredSoundEvents;
        auto categoryCooldowns = std::make_shared<std::vector<std::string> >(soundEventCooldowns);
        const auto catIt = categoriesMap.find(soundEventName);
        const auto subCatIt = categoriesMap.find(subSoundEventName);
        std::string s = strcmp(subSoundEventName.c_str(), "") == 0 ? soundEventName : subSoundEventName;
        LogDebug(std::format("Attempting to play sound '{}'", s));
        SoundEvent event;
        if (subCatIt != categoriesMap.end() && !subCatIt->second.files.empty()) {
            event = subCatIt->second;
        } else if (catIt != categoriesMap.end() && !catIt->second.files.empty()) {
            event = catIt->second;
        } else {
            return;
        }
        if (const std::unordered_set categoryCooldownsSet(categoryCooldowns->begin(), categoryCooldowns->end()); !categoryCooldownsSet.contains(soundEventName)) {
            const int chance = event.chance;
            float cooldown = event.cooldown;
            const int overrideMode = event.overrideMode;
            auto &files = event.files;
            if (generateRandomInt(0, 99) < chance) {
                auto it = files.begin();
                std::advance(it, generateRandomInt(0, static_cast<int>(files.size() - 1)));
                auto &[filePath, fileDuration] = *it;
                if (isSoundPlaying && overrideMode == 1) {
                    return;
                }
                StopSound();
                isSoundPlaying = true;
                categoryCooldowns->push_back(soundEventName);
                RunConsoleCommand("player.speaksound \"FX/LNTC_PlayerVoiceEvents/" + filePath + "\"");

                std::thread([fileDuration, cooldown, soundEventName, categoryCooldowns]() {
                    float f = fileDuration;
                    while (f > 0) {
                        if (!RE::UI::GetSingleton()->GameIsPaused()) f -= 0.010f;
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    isSoundPlaying = false;
                    f = cooldown - fileDuration;
                    while (f > 0) {
                        if (!RE::UI::GetSingleton()->GameIsPaused()) f -= 0.010f;
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    std::erase(*categoryCooldowns, soundEventName);
                }).detach();
            }
        }
    }
}
