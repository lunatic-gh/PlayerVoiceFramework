#include "../include/PVF/PapyrusInterface.h"

#include "../../include/PVF/SaveDataStorage.h"
#include "../../include/PVF/Util.h"
#include "../../include/PVF/SoundManager.h"

// Disable const warnings. The papyrus-interface internally breaks with const-values.
// ReSharper disable CppPassValueParameterByConstReference CppParameterMayBeConst CppParameterMayBeConstPtrOrRef
namespace PVF {

    bool PapyrusInterface::PapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
        vm->RegisterFunction("FormHasKeyword", "PlayerVoiceFramework", FormHasKeyword);
        vm->RegisterFunction("FormFromID", "PlayerVoiceFramework", FormFromID);
        vm->RegisterFunction("FormToString", "PlayerVoiceFramework", FormToString);
        vm->RegisterFunction("FormFromString", "PlayerVoiceFramework", FormFromString);
        vm->RegisterFunction("ReplaceInString", "PlayerVoiceFramework", ReplaceInString);
        vm->RegisterFunction("TrimString", "PlayerVoiceFramework", TrimString);
        vm->RegisterFunction("SendSoundEvent", "PlayerVoiceFramework", SendSoundEvent);
        vm->RegisterFunction("RandomInt", "PlayerVoiceFramework", RandomInt);
        vm->RegisterFunction("RandomFloat", "PlayerVoiceFramework", RandomFloat);
        vm->RegisterFunction("GetActivePack", "PlayerVoiceFramework", GetActivePack);
        vm->RegisterFunction("SetActivePack", "PlayerVoiceFramework", SetActivePack);
        vm->RegisterFunction("GetPacks", "PlayerVoiceFramework", GetPacks);
        return true;
    }

    void PapyrusInterface::Register() {
        SKSE::GetPapyrusInterface()->Register(PapyrusFunctions);
    }

    bool PapyrusInterface::FormHasKeyword(RE::StaticFunctionTag*, RE::TESForm* form, std::string keyword) {
        return FormUtil::HasKeyword(form, keyword);
    }

    RE::TESForm* PapyrusInterface::FormFromID(RE::StaticFunctionTag*, std::string pluginName, RE::FormID formId) {
        return FormUtil::FromID(pluginName, formId);
    }

    std::string PapyrusInterface::FormToString(RE::StaticFunctionTag*, RE::TESForm* form) {
        return FormUtil::ToString(form);
    }

    RE::TESForm* PapyrusInterface::FormFromString(RE::StaticFunctionTag*, std::string formString) {
        return FormUtil::FromString(formString);
    }

    std::string PapyrusInterface::ReplaceInString(RE::StaticFunctionTag*, std::string text, std::string oldSeq, std::string newSeq) {
        return StringUtil::ReplaceInString(text, oldSeq, newSeq);
    }

    std::string PapyrusInterface::TrimString(RE::StaticFunctionTag*, std::string text) {
        return StringUtil::TrimString(text);
    }

    void PapyrusInterface::SendSoundEvent(RE::StaticFunctionTag*, std::string name) {
        SoundManager::GetSingleton()->SendSoundEvent(name);
    }

    int PapyrusInterface::RandomInt(RE::StaticFunctionTag*, int minInclusive, int maxInclusive) {
        return Util::RandomInt(minInclusive, maxInclusive);
    }

    float PapyrusInterface::RandomFloat(RE::StaticFunctionTag*, float minInclusive, float maxInclusive) {
        return Util::RandomFloat(minInclusive, maxInclusive);
    }

    std::string PapyrusInterface::GetActivePack(RE::StaticFunctionTag*) {
        auto pack = Util::GetActivePack();
        Logger::LogInfo(pack);
        return pack;
    }

    void PapyrusInterface::SetActivePack(RE::StaticFunctionTag*, std::string name) {
        return Util::SetActivePack(name);
    }

    std::vector<std::string> PapyrusInterface::GetPacks(RE::StaticFunctionTag*) {
        if (const auto soundManager = SoundManager::GetSingleton()) { return soundManager->GetRegisteredPacks(); }
        return {};
    }
}