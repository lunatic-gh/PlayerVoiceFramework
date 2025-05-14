#include "../../include/PVF/Util.h"

#include "../../include/PVF/MemoryDataStorage.h"
#include "../../include/PVF/SaveDataStorage.h"
#include "../../include/PVF/SoundManager.h"

#include <yaml-cpp/yaml.h>
#include <ranges>

namespace PVF {
    bool FormUtil::HasKeyword(RE::TESForm* form, const std::string& keyword) {
        return form && form->HasKeywordByEditorID(keyword);
    }

    RE::TESForm* FormUtil::FromID(const std::string& pluginName, const RE::FormID& formId) {
        if (const auto dataHandler = RE::TESDataHandler::GetSingleton()) { return dataHandler->LookupForm(formId, pluginName); }
        return nullptr;
    }

    std::string FormUtil::ToString(const RE::TESForm* form) {
        if (form && form->GetFile()) { return std::format("{}|{:x}", form->GetFile()->GetFilename(), form->GetLocalFormID()); }
        return "";
    }

    RE::TESForm* FormUtil::FromString(const std::string& formString) {
        if (!formString.empty()) {
            if (const auto dataHandler = RE::TESDataHandler::GetSingleton()) {
                if (const auto s = StringUtil::SplitString(formString, '|'); s.size() >= 2) {
                    try {
                        return dataHandler->LookupForm(std::stoi(s[1], nullptr, 16), s[0]);
                    } catch (...) {}
                }
            }
        }
        return nullptr;
    }
    bool FormUtil::CompareForms(const std::string& first, const std::string& second) {
        const auto firstSplit = StringUtil::SplitString(first, '|');
        const auto secondSplit = StringUtil::SplitString(second, '|');
        if (firstSplit.size() == 2 && secondSplit.size() == 2) {
            if (const auto dataHandler = RE::TESDataHandler::GetSingleton()) {
                if (const auto firstForm = dataHandler->LookupForm(std::stoi(firstSplit[1], nullptr, 16), firstSplit[0])) {
                    if (const auto secondForm = dataHandler->LookupForm(std::stoi(secondSplit[1], nullptr, 16), secondSplit[0])) { return firstForm == secondForm; }
                }
            }
        }
        return false;
    }
    bool FormUtil::CompareForms(const RE::TESForm* first, const std::string& second) {
        if (first) { return first == FromString(second); }
        return false;
    }

    std::string FormUtil::ToKeywordString(RE::TESForm* form) {
        std::string s;
        if (form) {
            if (const auto keywordForm = form->As<RE::BGSKeywordForm>()) {
                const auto keywords = keywordForm->GetKeywords();
                for (int i = 0; i < keywords.size(); ++i) {
                    s += keywords[i]->GetFormEditorID();
                    if (i != keywords.size() - 1) { s += "|"; }
                }
            }
        }
        return s;
    }

    std::string StringUtil::ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq) {
        if (oldSeq.empty()) return text;
        std::string result = text;
        size_t pos = 0;
        while ((pos = result.find(oldSeq, pos)) != std::string::npos) {
            result.replace(pos, oldSeq.length(), newSeq);
            pos += newSeq.length();
        }
        return result;
    }

    std::string StringUtil::TrimString(const std::string& text) {
        auto view = std::string_view(text);
        view.remove_prefix(std::min(text.find_first_not_of(" \t\r\v\n"), text.size()));
        view.remove_suffix(std::min(text.size() - text.find_last_not_of(" \t\r\v\n") - 1, text.size()));
        return std::string(view);
    }

    std::vector<std::string> StringUtil::SplitString(const std::string& text, const char& delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(text);
        std::string s;
        while (std::getline(ss, s, delimiter))
            result.push_back(s);
        return result;
    }

    void Util::LoadData() {
        try {
            for (const auto& dir : std::filesystem::directory_iterator("Data/Sound/PlayerVoiceFramework/Packs", std::filesystem::directory_options::skip_permission_denied)) {
                if (dir.exists() && dir.is_directory()) {
                    const auto packName = dir.path().filename().string();
                    if (std::filesystem::path configPath = dir.path() / "config.yml"; std::filesystem::exists(configPath) && std::filesystem::is_regular_file(configPath)) {
                        YAML::Node rootNode;
                        try {
                            rootNode = YAML::LoadFile(configPath.string());
                        } catch (const YAML::ParserException&) {
                            Logger::LogError(std::format("Error while trying to parse yaml-config at '{}'", configPath.string()));
                            continue;
                        }
                        if (const auto events = rootNode["events"]; events && events.IsSequence()) {
                            for (auto event : events) {
                                if (event["name"] && event["name"].IsScalar() && event["audios"] && event["audios"].IsSequence()) {
                                    const auto name = event["name"].as<std::string>();
                                    std::vector<std::string> overrideBlacklist;
                                    if (event["overrideBlacklist"] && event["overrideBlacklist"].IsSequence()) {
                                        for (auto val : event["overrideBlacklist"]) {
                                            if (val.IsScalar()) { overrideBlacklist.push_back(val.as<std::string>()); }
                                        }
                                    }
                                    int chance = 100;
                                    int cooldown = 0;
                                    try {
                                        chance = event["chance"] && event["chance"].IsScalar() ? event["chance"].as<int>() : 100;
                                    } catch (const std::exception&) {}
                                    try {
                                        cooldown = event["cooldown"] && event["cooldown"].IsScalar() ? event["cooldown"].as<int>() : 0;
                                    } catch (const std::exception&) {}
                                    std::vector<std::pair<std::string, std::vector<std::string>>> audios;
                                    if (event["audios"] && event["audios"].IsSequence()) {
                                        for (auto audio : event["audios"]) {
                                            if (audio["files"] && audio["files"].IsSequence()) {
                                                std::vector<std::string> files;
                                                for (auto file : audio["files"]) {
                                                    files.push_back(file.as<std::string>());
                                                }
                                                const auto condition = audio["condition"] && audio["condition"].IsScalar() ? audio["condition"].as<std::string>() : "";
                                                audios.emplace_back(condition, files);
                                            }
                                        }
                                    }
                                    if (auto soundManager = SoundManager::GetSingleton()) soundManager->RegisterSoundEvent(packName, name, chance, cooldown, overrideBlacklist, audios);
                                }
                            }
                        }
                        Logger::LogInfo(std::format("Loaded Voice-Pack '{}'", packName));
                    } else {
                        Logger::LogDebug(std::format("Found Voice-Pack without Config at '{}', ignoring...", dir.path().string()));
                        continue;
                    }
                }
            }
        } catch (const std::exception& e) { Logger::LogError(std::format("Unexpected Error trying to load data: '{}'", e.what())); }
    }

    int Util::RandomInt(const int minInclusive, const int maxInclusive) {
        std::random_device dev;
        std::mt19937 gen(dev());
        std::uniform_int_distribution range(minInclusive, maxInclusive);
        return range(gen);
    }

    float Util::RandomFloat(const float minInclusive, const float maxInclusive) {
        std::random_device dev;
        std::mt19937 gen(dev());
        std::uniform_real_distribution range(minInclusive, maxInclusive);
        return range(gen);
    }

    std::string Util::GetActivePack() {
        if (const auto saveDataStorage = SaveDataStorage::GetSingleton()) {
            auto s = saveDataStorage->Get("activePack").AsString();
            return s;
        }
        return "";
    }

    void Util::SetActivePack(const std::string& name) {
        if (const auto saveDataStorage = SaveDataStorage::GetSingleton()) {
            if (name.empty()) {
                saveDataStorage->Remove("activePack");
                return;
            }
            saveDataStorage->Set("activePack", DataValue(name.empty() ? "" : name.c_str()));
        }
    }

    void Util::LoadSetting(const YAML::Node& node, const std::string& key, const DataValue& def, const std::string& type) {
        if (const auto keyNode = node[key]) {
            switch (keyNode.Type()) {
                case YAML::NodeType::Scalar: {
                    if (type == "string") {
                        const auto s = keyNode.as<std::string>();
                        const auto data = DataValue(s.c_str());
                        MemoryDataStorage::GetSingleton()->Set(std::format("{}", key), data);
                        Logger::LogInfo(std::format("2 Loaded Setting '{}' with value '{}'", key, data.AsString()));

                    } else if (type == "int") {
                        const auto data = DataValue(keyNode.as<int>());
                        MemoryDataStorage::GetSingleton()->Set(std::format("{}", key), data);
                        Logger::LogInfo(std::format("2 Loaded Setting '{}' with value '{}'", key, data.AsInt()));
                    } else if (type == "float") {
                        const auto data = DataValue(keyNode.as<float>());
                        MemoryDataStorage::GetSingleton()->Set(std::format("{}", key), data);
                        Logger::LogInfo(std::format("3 Loaded Setting '{}' with value '{}'", key, data.AsFloat()));
                    }
                    return;
                }
                default:
                    Logger::LogError(std::format("Setting '{}' is of invalid type, expected '{}'", key, type));
                    break;
            }
            MemoryDataStorage::GetSingleton()->Set(std::format("{}", key), def);
            if (def.GetType() == DataValue::Type::kString) {
                Logger::LogInfo(std::format("Loaded Setting '{}' with default value '{}'", key, def.AsString()));
            } else if (def.GetType() == DataValue::Type::kInt) {
                Logger::LogInfo(std::format("Loaded Setting '{}' with default value '{}'", key, def.AsInt()));
            } else if (def.GetType() == DataValue::Type::kFloat) {
                Logger::LogInfo(std::format("Loaded Setting '{}' with default value '{}'", key, def.AsFloat()));
            } else if (def.GetType() == DataValue::Type::kBool) {
                Logger::LogInfo(std::format("Loaded Setting '{}' with default value '{}'", key, def.AsBool()));
            }
        }
    }
}