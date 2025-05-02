#include "../include/PVE/API.h"

#include "../../include/PVE/ConditionManager.h"
#include "../../include/PVE/SoundManager.h"
#include "../../include/PVE/Util.h"
#include "../../include/PVE/MemoryDataStorage.h"
#include "../../include/PVE/SaveDataStorage.h"

/**
 * NOTE TO MYSELF: DON'T CHANGE EXISTING DEFINITIONS UNLESS ABSOLUTELY NECESSARY!
 * RATHER DEPRECATE THEM.
 */
namespace PVE_API {

    // 1.0.3: Added
    void PlayerVoiceEventsAPI::SendSoundEvent(const std::string& name) {
        PVE::SoundManager::GetSingleton()->SendSoundEvent(name);
    }

    // 1.0.3: Added
    void PlayerVoiceEventsAPI::RegisterCondition(const std::string& eventName, const std::string& conditionName, const std::function<std::variant<float, int, bool, std::string, RE::TESForm*>()>& conditionFunction) {
        if (const auto conditionManager = PVE::ConditionManager::GetSingleton()) {
            conditionManager->RegisterCondition(eventName, conditionName, conditionFunction);
        }
    }

    // 1.0.3: Added
    void PlayerVoiceEventsAPI::SetMemoryData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value) {
        if (const auto memoryDataStorage = PVE::MemoryDataStorage::GetSingleton()) memoryDataStorage->Set(key, value);
    }

    // 1.0.3: Added
    std::variant<std::string, int, float, RE::TESForm*> PlayerVoiceEventsAPI::GetMemoryData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def) {
        if (const auto memoryDataStorage = PVE::MemoryDataStorage::GetSingleton()) {
            if (std::holds_alternative<std::string>(def)) {
                return memoryDataStorage->Get<std::string>(key, std::get<std::string>(def));
            }
            if (std::holds_alternative<int>(def)) {
                return memoryDataStorage->Get<int>(key, std::get<int>(def));
            }
            if (std::holds_alternative<float>(def)) {
                return memoryDataStorage->Get<float>(key, std::get<float>(def));
            }
            if (std::holds_alternative<RE::TESForm*>(def)) {
                return memoryDataStorage->Get<RE::TESForm*>(key, std::get<RE::TESForm*>(def));
            }
        }
        return def;
    }

    // 1.0.3: Added
    void PlayerVoiceEventsAPI::SetSaveData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value) {
        if (const auto saveDataStorage = PVE::SaveDataStorage::GetSingleton()) {
            saveDataStorage->Set(key, value);
        }
    }

    // 1.0.3: Added
    std::variant<std::string, int, float, RE::TESForm*> PlayerVoiceEventsAPI::GetSaveData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def) {
        if (const auto saveDataStorage = PVE::SaveDataStorage::GetSingleton()) {
            if (std::holds_alternative<std::string>(def)) {
                return saveDataStorage->Get<std::string>(key, std::get<std::string>(def));
            }
            if (std::holds_alternative<int>(def)) {
                return saveDataStorage->Get<int>(key, std::get<int>(def));
            }
            if (std::holds_alternative<float>(def)) {
                return saveDataStorage->Get<float>(key, std::get<float>(def));
            }
            if (std::holds_alternative<RE::TESForm*>(def)) {
                return saveDataStorage->Get<RE::TESForm*>(key, std::get<RE::TESForm*>(def));
            }
        }
        return def;
    }

    // 1.0.3: Added
    bool PlayerVoiceEventsAPI::FormHasKeyword(RE::TESForm* form, const std::string& keyword) {
        return PVE::FormUtil::HasKeyword(form, keyword);
    }

    // 1.0.3: Added
    std::string PlayerVoiceEventsAPI::FormToString(const RE::TESForm* form) {
        return PVE::FormUtil::ToString(form);
    }

    // 1.0.3: Added
    RE::TESForm* PlayerVoiceEventsAPI::FormFromString(const std::string& formString) {
        return PVE::FormUtil::FromString(formString);
    }

    // 1.0.3: Added
    RE::TESForm* PlayerVoiceEventsAPI::FormFromID(const std::string& pluginName, const RE::FormID& formId) {
        return PVE::FormUtil::FromID(pluginName, formId);
    }

    // 1.0.3: Added
    bool PlayerVoiceEventsAPI::CompareForms(const std::string& first, const std::string& second) {
        return PVE::FormUtil::CompareForms(first, second);
    }

    // 1.0.3: Added
    bool PlayerVoiceEventsAPI::CompareForms(const RE::TESForm* first, const std::string& second) {
        return PVE::FormUtil::CompareForms(first, second);
    }

    // 1.0.3: Added
    std::string PlayerVoiceEventsAPI::FormToKeywordString(RE::TESForm* form) {
        return PVE::FormUtil::ToKeywordString(form);
    }

    // 1.0.3: Added
    std::string PlayerVoiceEventsAPI::ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq) {
        return PVE::StringUtil::ReplaceInString(text, oldSeq, newSeq);
    }

    // 1.0.3: Added
    std::string PlayerVoiceEventsAPI::TrimString(const std::string& text) {
        return PVE::StringUtil::TrimString(text);
    }

    // 1.0.3: Added
    std::vector<std::string> PlayerVoiceEventsAPI::SplitString(const std::string& text, const char& delimiter) {
        return PVE::StringUtil::SplitString(text, delimiter);
    }

    // 1.0.3: Added
    int PlayerVoiceEventsAPI::RandomInt(const int minInclusive, const int maxInclusive) {
        return PVE::Util::RandomInt(minInclusive, maxInclusive);
    }

    // 1.0.3: Added
    float PlayerVoiceEventsAPI::RandomFloat(const float minInclusive, const float maxInclusive) {
        return PVE::Util::RandomFloat(minInclusive, maxInclusive);
    }
}