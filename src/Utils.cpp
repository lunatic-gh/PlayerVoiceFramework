#include "Utils.h"

#include <nlohmann/json.hpp>

#include "Logger.h"
#include "SoundEvent.h"

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
                        Log(std::format("Warning: Could not open File '{}'", entry.path().string()));
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
                        Log(std::format("Warning: Could not open File '{}'", entry.path().string()));
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
                        quests.push_back(std::make_tuple(name, questRef));
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
            Log(std::format("Warning: Could not open Voice-Pack Configuration at '{}'", packConfigPath));
            return;
        }
        nlohmann::json json;
        try {
            packConfigFile >> json;
        } catch (const nlohmann::json::parse_error &) {
            Log("Error: Could not parse Voice-Pack Configuration - Please make sure it is in proper json-format.");
            return;
        }
        for (const auto &[key, value] : json["sounds"].items()) {
            std::vector<std::pair<std::string, std::vector<std::string>>> audios;
            if (value.contains("audio") && value.at("audio").is_array()) {
                for (auto audio : value.at("audio").get<std::vector<nlohmann::basic_json<>>>()) {
                    std::string condition = "";
                    std::vector<std::string> files;
                    if (!audio.contains("files") || !audio["files"].is_array() || audio["files"].empty()) {
                        continue;
                    }
                    files = audio.at("files").get<std::vector<std::string>>();
                    if (audio.contains("condition") && audio.at("condition").is_string() && !audio.at("condition").get<std::string>().empty()) {
                        condition = audio.at("condition").get<std::string>();
                    }
                    audios.push_back(std::make_pair(condition, files));
                }
            }
            if (audios.empty()) {
                continue;
            }
            SoundEvent soundEvent(
                value.contains("chance") && value.at("chance").is_number_integer() ? value.at("chance").get<int>() : 100,
                value.contains("cooldown") && value.at("cooldown").is_number_float() ? value.at("cooldown").get<float>() : 0.0f,
                value.contains("canBeOverridden") && value.at("canBeOverridden").is_boolean() ? value.at("canBeOverridden").get<bool>() : false,
                value.contains("forceOverrideOthers") && value.at("forceOverrideOthers").is_boolean() ? value.at("forceOverrideOthers").get<bool>() : false,
                value.contains("delay") && value.at("delay").is_number_float() ? value.at("delay").get<float>() : 0.0f,
                value.contains("volume") && value.at("volume").is_number_float() ? value.at("volume").get<float>() : 1.0f,
                audios);
            registeredSoundEvents[key] = soundEvent;
            eventCooldowns[key] = false;
            Log(std::format("Loaded Sound-Data for '{}'", key));
        }
        Log("Done Loading...");
        packConfigFile.close();
    }

    void Utils::PlaySound(const std::string &soundEventName, const std::string &subSoundEventName) {
        auto player = RE::PlayerCharacter::GetSingleton();
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
        Log(std::format("Received Event '{}'", s));
        SoundEvent event;
        if (subEventIt != registeredSoundEvents.end()) {
            event = subEventIt->second;
        } else if (eventIt != registeredSoundEvents.end()) {
            event = eventIt->second;
        } else {
            return;
        }
        std::thread([event, soundEventName, player]() mutable {
            float delaySeconds = event.GetDelay();
            while (delaySeconds > 0.0f) {
                if (!RE::UI::GetSingleton()->GameIsPaused() && player->Is3DLoaded()) {
                    delaySeconds -= 0.05f;
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
            if (!currentSound.has_value() || !currentSound->IsPlaying() || (currentSound->CanBeOverridden() || event.IsForceOverrideOthers())) {
                if (eventCooldowns[soundEventName] == 0.0f) {
                    if (GenerateRandomInt(0, 99) < event.GetChance()) {
                        if (currentSound.has_value()) {
                            currentSound->Stop();
                        }
                        if (event.Play()) {
                            currentSound.emplace(event);
                            eventCooldowns[soundEventName] = event.GetCooldown();
                        }
                    }
                }
            }
        }).detach();
    }

    void Utils::Log(const std::string &msg) {
        logger::info("{}", msg);
    }

    std::string Utils::Replace(const std::string &text, const std::string &oldSeq, const std::string &newSeq) {
        if (oldSeq.empty()) {
            return text;
        }
        std::string result = text;
        size_t pos = 0;
        while ((pos = result.find(oldSeq, pos)) != std::string::npos) {
            result.replace(pos, oldSeq.length(), newSeq);
            pos += newSeq.length();
        }
        return result;
    }

    int Utils::GenerateRandomInt(const int minInclusive, const int maxInclusive) {
        std::random_device randomDev;
        std::mt19937 randomGen(randomDev());
        std::uniform_int_distribution<> randomRange(minInclusive, maxInclusive);
        return randomRange(randomGen);
    }

    void Utils::RunConsoleCommand(const std::string &command) {
        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        if (const auto script = scriptFactory ? scriptFactory->Create() : nullptr) {
            script->SetCommand(command);
            CompileAndRun(script, GetSelectedRef().get());
            delete script;
        }
    }

    bool Utils::FormHasKeywordString(RE::TESForm *form, const std::string &keyword) {
        if (!form) return false;
        const auto keywordForm = form->As<RE::BGSKeywordForm>();
        if (!keywordForm) return false;
        return keywordForm->HasKeywordString(keyword);
    }
    float Utils::CalculateDistance(const float x1, const float y1, const float x2, const float y2) {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    int Utils::QueryLocationChange(const std::tuple<std::string, std::string, float, float, float, float> &locData) {
        const auto player = RE::PlayerCharacter::GetSingleton();

        const float x = player->GetPositionX();
        const float y = player->GetPositionY();

        auto [locName, worldspaceName, locX, locY, locRadiusEnter, locRadiusLeave] = locData;
        if (player->GetWorldspace() && player->GetWorldspace()->GetFullName() == worldspaceName) {
            const float dist = CalculateDistance(x, y, locX, locY);
            if (!currentLocation.has_value() || dist < locRadiusEnter && locName != currentLocation.value()) {
                currentLocation.emplace(locName);
                return 2;
            }
            if (currentLocation.has_value() && dist > (locRadiusLeave) && locName == currentLocation.value()) {
                currentLocation.emplace("");
                return 1;
            }
        }
        return 0;
    }

    std::vector<std::string> Utils::SplitByChar(const std::string &input, const char &delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string s;
        while (std::getline(ss, s, delimiter)) result.push_back(s);
        return result;
    }
    std::string Utils::TrimString(const std::string &s) {
        auto view = std::string_view(s);
        view.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
        view.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));
        return std::string(view);
    }

    void Utils::CompileAndRun(RE::Script *script, RE::TESObjectREFR *targetRef, const RE::COMPILER_NAME name) {
        RE::ScriptCompiler compiler;
        CompileAndRunImpl(script, &compiler, name, targetRef);
    }

    void Utils::CompileAndRunImpl(RE::Script *script, RE::ScriptCompiler *compiler, RE::COMPILER_NAME name, RE::TESObjectREFR *targetRef) {
        using func_t = decltype(CompileAndRunImpl);
        const REL::Relocation<func_t> func{RELOCATION_ID(21416, REL::Module::get().version().patch() < 1130 ? 21890 : 441582)};
        return func(script, compiler, name, targetRef);
    }

    RE::NiPointer<RE::TESObjectREFR> Utils::GetSelectedRef() {
        const auto handle = GetSelectedRefHandle();
        return handle.get();
    }

    RE::ObjectRefHandle Utils::GetSelectedRefHandle() {
        const REL::Relocation<RE::ObjectRefHandle *> selectedRef{RELOCATION_ID(519394, REL::Module::get().version().patch() < 1130 ? 405935 : 504099)};
        return *selectedRef;
    }
}