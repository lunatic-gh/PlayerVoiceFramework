#include "Utils.h"

#include <nlohmann/json.hpp>

namespace PVE {
    void Utils::LoadData() {
        // Load Location Data
        const std::string locDataDirPath = "Data/Sound/PlayerVoiceEvents/LocationData";
        for (const auto &entry : std::filesystem::directory_iterator(locDataDirPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".json") {
                    std::ifstream locDataFile(entry.path());
                    if (!locDataFile.is_open()) {
                        Log(std::format("Warning: Could not open File '{}' - This should not happen...", entry.path().string()));
                        continue;
                    }
                    nlohmann::json json;
                    try {
                        locDataFile >> json;
                    } catch (const nlohmann::json::parse_error &) {
                        Log(std::format("Warning: Could not open File '{}' - Please make sure it is in proper json-format.", entry.path().string()));
                        continue;
                    }

                    if (!(json.contains("enabled") && json.at("enabled").is_boolean() ? json.at("enabled").get<bool>() : true)) {
                        continue;
                    }
                    if (json.contains("name") && json.at("name").is_string() && json.contains("worldspace") && json.at("worldspace").is_string() &&
                        json.contains("locX") && json.at("locX").is_number_float() && json.contains("locY") && json.at("locY").is_number_float() &&
                        json.contains("locDistEnter") && json.at("locDistEnter").is_number_float() && json.contains("locDistLeave") &&
                        json.at("locDistLeave").is_number_float()) {
                        auto name = json.at("name").get<std::string>();
                        auto worldspace = json.at("worldspace").get<std::string>();
                        float locX = json.at("locX").get<float>();
                        float locY = json.at("locY").get<float>();
                        float locDistEnter = json.at("locDistEnter").get<float>();
                        float locDistLeave = json.at("locDistLeave").get<float>();
                        locations.push_back(std::make_tuple(name, worldspace, locX, locY, locDistEnter, locDistLeave));
                        Log(std::format("Loaded Location-Data for '{}'", name));
                    } else {
                        Log(std::format("Warning: Ignoring Data File '{}', because it has missing or corrupted data. Please check it for errors.",
                                        entry.path().string()));
                    }
                    locDataFile.close();
                }
            }
        }

        // Load Quest Data
        const std::string questDataDirPath = "Data/Sound/PlayerVoiceEvents/QuestData";
        for (const auto &entry : std::filesystem::directory_iterator(questDataDirPath)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".json") {
                    std::ifstream questDataFile(entry.path());
                    if (!questDataFile.is_open()) {
                        Log(std::format("Warning: Could not open File '{}' - This should not happen...", entry.path().string()));
                        continue;
                    }

                    nlohmann::json json;
                    try {
                        questDataFile >> json;
                    } catch (const nlohmann::json::parse_error &) {
                        Log(std::format("Warning: Could not open File '{}' - Please make sure it is in proper json-format.", entry.path().string()));
                        continue;
                    }

                    if (!(json.contains("enabled") && json.at("enabled").is_boolean() ? json.at("enabled").get<bool>() : true)) {
                        continue;
                    }
                    if (json.contains("name") && json.at("name").is_string() && json.contains("questRef") && json.at("questRef").is_string()) {
                        auto name = json.at("name").get<std::string>();
                        auto split = SplitByChar(json.at("questRef").get<std::string>(), '|');
                        auto questRef = std::make_pair(split[0], std::stoi(split[1], nullptr, 16));
                        std::map<int, float> stageDelays;
                        if (json.contains("stageDelays") && json.at("stageDelays").is_object()) {
                            for (auto [key, value] : json["stageDelays"].items()) {
                                stageDelays[std::stoi(key)] = value.is_number_float() ? static_cast<float>(value) : 0.0f;
                            }
                        }
                        quests.push_back(std::make_tuple(name, questRef, stageDelays));
                        Log(std::format("Loaded Quest-Data for '{}'", name));
                    } else {
                        Log(std::format("Warning: Ignoring Data File '{}', because it has missing or corrupted data. Please check it for errors.",
                                        entry.path().string()));
                    }
                    questDataFile.close();
                }
            }
        }

        // Load Sound Data
        const std::string packConfigPath = "Data/Sound/PlayerVoiceEvents/SoundData/config.json";
        std::ifstream packConfigFile(packConfigPath);
        if (!packConfigFile.is_open()) {
            Log(std::format("Warning: Could not open File '{}' - If you don't have a Voice-Pack installed, this can be safely ignored.", packConfigPath));
            return;
        }
        nlohmann::json json;
        try {
            packConfigFile >> json;
        } catch (const nlohmann::json::parse_error &) {
            Log("Error: Could not parse Voice-Pack Configuration - Please make sure it is in proper json-format.");
            return;
        }
        std::map<std::string, SoundEvent> soundEvents;
        for (auto &[key, value] : json["sounds"].items()) {
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
        packConfigFile.close();
    }

    void Utils::PlaySound(const std::string &soundEventName, const std::string &subSoundEventName) {
        const auto player = RE::PlayerCharacter::GetSingleton();
        // If not loaded into the world (e.g. in loading screens)
        if (!player->Is3DLoaded()) {
            return;
        }
        // If Vampire-Lord or Werewolf
        if (player->GetRace()->GetFormID() == 0xCDD84 || player->GetRace()->GetFormID() == 0x283A) {
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
                    RunConsoleCommand("player.speaksound \"PlayerVoiceEvents/SoundData/" + file + "\"");
                }
            }
        }
    }
}