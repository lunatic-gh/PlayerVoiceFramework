#include "../include/PVF/API.h"

#include "../../include/PVF/ConditionManager.h"
#include "../../include/PVF/SoundManager.h"
#include "../../include/PVF/Util.h"
#include "../../include/PVF/MemoryDataStorage.h"
#include "../../include/PVF/SaveDataStorage.h"

namespace PVF_API {

    void PlayerVoiceFrameworkAPI::SendSoundEvent(const char* name) {
        PVF::SoundManager::GetSingleton()->SendSoundEvent(name);
    }

    void PlayerVoiceFrameworkAPI::RegisterCondition(const char* eventName, const char* conditionName, const ConditionFunction& conditionFunction) {
        PVF::ConditionManager::GetSingleton()->RegisterCondition(eventName, conditionName, [conditionFunction] { return conditionFunction.function(); });
    }

    void PlayerVoiceFrameworkAPI::RegisterGlobalCondition(const char* conditionName, const ConditionFunction& conditionFunction) {
        PVF::ConditionManager::GetSingleton()->RegisterGlobalCondition(conditionName, [conditionFunction] { return conditionFunction.function(); });
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