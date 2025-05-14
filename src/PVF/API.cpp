#include "../include/PVF/API.h"

#include "../../include/PVF/ConditionManager.h"
#include "../../include/PVF/SoundManager.h"
#include "../../include/PVF/Util.h"

namespace PVF_API {

    void PlayerVoiceFrameworkAPI::LogInfo(const char* message) {
        PVF::Logger::LogInfo(message);
    }

    void PlayerVoiceFrameworkAPI::LogWarn(const char* message) {
        PVF::Logger::LogWarn(message);
    }

    void PlayerVoiceFrameworkAPI::LogError(const char* message) {
        PVF::Logger::LogError(message);
    }

    void PlayerVoiceFrameworkAPI::LogCritical(const char* message) {
        PVF::Logger::LogCritical(message);
    }

    void PlayerVoiceFrameworkAPI::LogDebug(const char* message) {
        PVF::Logger::LogDebug(message);
    }

    void PlayerVoiceFrameworkAPI::SendSoundEvent(const char* name) {
        PVF::SoundManager::GetSingleton()->SendSoundEvent(name);
    }

    void PlayerVoiceFrameworkAPI::RegisterCondition(const char* eventName, const char* conditionName, const PVF::ConditionFunction& conditionFunction) {
        PVF::ConditionManager::GetSingleton()->RegisterCondition(eventName, conditionName, conditionFunction);
    }

    void PlayerVoiceFrameworkAPI::RegisterGlobalCondition(const char* conditionName, const PVF::ConditionFunction& conditionFunction) {
        PVF::ConditionManager::GetSingleton()->RegisterGlobalCondition(conditionName, conditionFunction);
    }

    bool PlayerVoiceFrameworkAPI::FormHasKeyword(RE::TESForm* form, const char* keyword) {
        return PVF::FormUtil::HasKeyword(form, keyword);
    }

    int PlayerVoiceFrameworkAPI::RandomInt(const int minInclusive, const int maxInclusive) {
        return PVF::Util::RandomInt(minInclusive, maxInclusive);
    }

    float PlayerVoiceFrameworkAPI::RandomFloat(const float minInclusive, const float maxInclusive) {
        return PVF::Util::RandomFloat(minInclusive, maxInclusive);
    }
}