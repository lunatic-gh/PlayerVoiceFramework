#include "../include/PVE.h"

namespace PVE {
    bool FormUtil::HasKeyword(RE::TESForm* form, const std::string& keyword) {
        return form && form->HasKeywordByEditorID(keyword);
    }

    RE::TESForm* FormUtil::FromID(const std::string& pluginName, const RE::FormID& formId) {
        if (const auto dataHandler = RE::TESDataHandler::GetSingleton()) {
            return dataHandler->LookupForm(formId, pluginName);
        }
        return nullptr;
    }

    std::string FormUtil::ToString(const RE::TESForm* form) {
        if (form && form->GetFile()) {
            return std::format("{}|{}", form->GetFile()->GetFilename(), form->GetLocalFormID());
        }
        return "";
    }

    RE::TESForm* FormUtil::FromString(const std::string& formString) {
        if (!formString.empty()) {
            if (const auto dataHandler = RE::TESDataHandler::GetSingleton()) {
                if (const auto s = StringUtil::Split(formString, '|'); s.size() >= 2) {
                    try {
                        return dataHandler->LookupForm(std::stoi(s[1]), s[0]);
                    } catch (...) {
                    }
                }
            }
        }
        return nullptr;
    }
    bool FormUtil::CompareForms(const std::string& first, const std::string& second) {
        const auto firstSplit = StringUtil::Split(first, '|');
        const auto secondSplit = StringUtil::Split(second, '|');
        if (firstSplit.size() == 2 && secondSplit.size() == 2) {
            if (const auto dataHandler = RE::TESDataHandler::GetSingleton()) {
                if (const auto firstForm = dataHandler->LookupForm(std::stoi(firstSplit[1], nullptr, 16), firstSplit[0])) {
                    if (const auto secondForm = dataHandler->LookupForm(std::stoi(secondSplit[1], nullptr, 16), secondSplit[0])) {
                        return firstForm == secondForm;
                    }
                }
            }
        }
        return false;
    }
    bool FormUtil::CompareForms(const RE::TESForm* first, const std::string& second) {
        if (first) {
            return first == FromString(second);
        }
        return false;
    }
    std::string FormUtil::ToKeywordString(RE::TESForm* form) {
        std::string s = "";
        if (form) {
            if (const auto keywordForm = form->As<RE::BGSKeywordForm>()) {
                const auto keywords = keywordForm->GetKeywords();
                for (int i = 0; i < keywords.size(); ++i) {
                    s += keywords[i]->GetFormEditorID();
                    if (i != keywords.size() - 1) {
                        s += "|";
                    }
                }
            }
        }
        return s;
    }

    std::string StringUtil::ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq) {
        if (oldSeq.empty())
            return text;
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

    std::vector<std::string> StringUtil::Split(const std::string& text, const char& delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(text);
        std::string s;
        while (std::getline(ss, s, delimiter))
            result.push_back(s);
        return result;
    }

    void Util::LoadData() {
        if (const std::filesystem::path configPath{"Data/Sound/PlayerVoiceEvents/SoundData/config.yml"}; exists(configPath) && is_regular_file(configPath)) {
            try {
                if (YAML::Node config = YAML::LoadFile(configPath.string())) {
                    if (config["sounds"]) {
                        for (const YAML::Node soundNode : config["sounds"]) {
                            if (soundNode["name"]) {
                                const std::string name = soundNode["name"].as<std::string>();
                                const int chance = soundNode["chance"] ? soundNode["chance"].as<int>() : 100;
                                const float cooldown = soundNode["cooldown"] ? soundNode["cooldown"].as<float>() : 0.0f;
                                std::vector<std::string> overrideBlacklist;
                                if (soundNode["overrideBlacklist"]) {
                                    if (soundNode["overrideBlacklist"].IsSequence()) {
                                        for (const auto& entry : soundNode["overrideBlacklist"]) {
                                            overrideBlacklist.push_back(entry.as<std::string>());
                                        }
                                    }
                                }
                                if (soundNode["audios"]) {
                                    std::vector<std::pair<std::string, std::vector<std::string>>> audios;
                                    for (const auto audio : soundNode["audios"]) {
                                        auto condition = audio["condition"] ? audio["condition"].as<std::string>() : "";
                                        if (audio["files"]) {
                                            std::vector<std::string> files;
                                            for (const auto& fileNode : audio["files"]) {
                                                files.push_back(fileNode.as<std::string>());
                                            }
                                            audios.emplace_back(condition, files);
                                        }
                                    }
                                    SoundManager::GetSingleton().RegisterSoundEvent(name, chance, cooldown, overrideBlacklist, audios);
                                    LogDebug("Loaded Event {}", name);
                                }
                            }
                        }
                    }
                }
            } catch (const YAML::Exception&) {
                Util::LogError("Could not parse Yaml Config...");
            }
        } else {
            LogInfo("Config file not found: {}", configPath.string());
        }
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
}