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

    int PlayerVoiceEventsAPI::GetAPIVersionMajor() {
        return API_VERSION_MAJOR;
    }

    int PlayerVoiceEventsAPI::GetAPIVersionMinor() {
        return API_VERSION_MINOR;
    }

    void PlayerVoiceEventsAPI::SendSoundEvent(const char* name) {
        PVE::SoundManager::GetSingleton()->SendSoundEvent(name);
    }

    void PlayerVoiceEventsAPI::RegisterCondition(const char* eventName, const char* conditionName, const ConditionFunction& conditionFunction) {
        PVE::ConditionManager::GetSingleton()->RegisterCondition(eventName, conditionName, [conditionFunction] { return conditionFunction.function(); });
    }

    void PlayerVoiceEventsAPI::RegisterGlobalCondition(const char* conditionName, const ConditionFunction& conditionFunction) {
        PVE::ConditionManager::GetSingleton()->RegisterGlobalCondition(conditionName, [conditionFunction] { return conditionFunction.function(); });
    }

    void PlayerVoiceEventsAPI::SetMemoryData(const char* key, const PVE::DataType& value) {
        PVE::MemoryDataStorage::GetSingleton()->Set(key, value);
    }

    PVE::DataType PlayerVoiceEventsAPI::GetMemoryData(const char* key, const PVE::DataType& def) {
        return PVE::MemoryDataStorage::GetSingleton()->Get(key, def);
    }

    void PlayerVoiceEventsAPI::SetSaveData(const char* key, const PVE::DataType& value) {
        PVE::MemoryDataStorage::GetSingleton()->Set(key, value);
    }

    PVE::DataType PlayerVoiceEventsAPI::GetSaveData(const char* key, PVE::DataType def) {
        if (const auto saveDataStorage = PVE::SaveDataStorage::GetSingleton()) {
            return saveDataStorage->Get(key, def);
        }
        return def;
    }

    bool PlayerVoiceEventsAPI::FormHasKeyword(RE::TESForm* form, const char* keyword) {
        return PVE::FormUtil::HasKeyword(form, keyword);
    }

    int PlayerVoiceEventsAPI::RandomInt(const int minInclusive, const int maxInclusive) {
        return PVE::Util::RandomInt(minInclusive, maxInclusive);
    }

    float PlayerVoiceEventsAPI::RandomFloat(const float minInclusive, const float maxInclusive) {
        return PVE::Util::RandomFloat(minInclusive, maxInclusive);
    }
}