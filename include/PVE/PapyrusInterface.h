#pragma once

#include "SoundManager.h"

namespace PVE {
    class PapyrusInterface {
    public:
        static void Register() {
            SKSE::GetPapyrusInterface()->Register(PapyrusFunctions);
        }

    private:
        // ReSharper disable CppPassValueParameterByConstReference CppParameterMayBeConst CppParameterMayBeConstPtrOrRef
        static bool FormHasKeyword(RE::StaticFunctionTag*, RE::TESForm* form, std::string keyword) {
            return FormUtil::HasKeyword(form, keyword);
        }

        static RE::TESForm* FormFromID(RE::StaticFunctionTag*, std::string pluginName, RE::FormID formId) {
            return FormUtil::FromID(pluginName, formId);
        }

        static std::string FormToString(RE::StaticFunctionTag*, RE::TESForm* form) {
            return FormUtil::ToString(form);
        }

        static RE::TESForm* FormFromString(RE::StaticFunctionTag*, std::string formString) {
            return FormUtil::FromString(formString);
        }

        static std::string ReplaceInString(RE::StaticFunctionTag*, std::string text, std::string oldSeq, std::string newSeq) {
            return StringUtil::ReplaceInString(text, oldSeq, newSeq);
        }

        static std::string TrimString(RE::StaticFunctionTag*, std::string text) {
            return StringUtil::TrimString(text);
        }

        static bool PlaySoundEvent(RE::StaticFunctionTag*, std::string name) {
            return SoundManager::GetSingleton().SendSoundEvent(name);
        }

        static int RandomInt(RE::StaticFunctionTag*, int minInclusive, int maxInclusive) {
            return Util::RandomInt(minInclusive, maxInclusive);
        }

        static float RandomFloat(RE::StaticFunctionTag*, float minInclusive, float maxInclusive) {
            return Util::RandomFloat(minInclusive, maxInclusive);
        }
        // ReSharper restore CppPassValueParameterByConstReference CppParameterMayBeConst CppParameterMayBeConstPtrOrRef

        static bool PapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
            vm->RegisterFunction("FormHasKeyword", "PVEPapyrusUtil", FormHasKeyword);
            vm->RegisterFunction("FormFromID", "PVEPapyrusUtil", FormFromID);
            vm->RegisterFunction("FormToString", "PVEPapyrusUtil", FormToString);
            vm->RegisterFunction("FormFromString", "PVEPapyrusUtil", FormFromString);
            vm->RegisterFunction("ReplaceInString", "PVEPapyrusUtil", ReplaceInString);
            vm->RegisterFunction("TrimString", "PVEPapyrusUtil", TrimString);
            vm->RegisterFunction("PlaySoundEvent", "PVEPapyrusUtil", PlaySoundEvent);
            vm->RegisterFunction("RandomInt", "PVEPapyrusUtil", RandomInt);
            vm->RegisterFunction("RandomFloat", "PVEPapyrusUtil", RandomFloat);
            return true;
        }
    };
}