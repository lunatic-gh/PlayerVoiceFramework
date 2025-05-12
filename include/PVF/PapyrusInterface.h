#pragma once

namespace PVF {
    class PapyrusInterface {
    public:
        static void Register();

    private:
        static bool PapyrusFunctions(RE::BSScript::IVirtualMachine* vm);

        static bool FormHasKeyword(RE::StaticFunctionTag*, RE::TESForm* form, std::string keyword);

        static RE::TESForm* FormFromID(RE::StaticFunctionTag*, std::string pluginName, RE::FormID formId);

        static std::string FormToString(RE::StaticFunctionTag*, RE::TESForm* form);

        static RE::TESForm* FormFromString(RE::StaticFunctionTag*, std::string formString);

        static std::string ReplaceInString(RE::StaticFunctionTag*, std::string text, std::string oldSeq, std::string newSeq);

        static std::string TrimString(RE::StaticFunctionTag*, std::string text);

        static void SendSoundEvent(RE::StaticFunctionTag*, std::string name);

        static int RandomInt(RE::StaticFunctionTag*, int minInclusive, int maxInclusive);

        static float RandomFloat(RE::StaticFunctionTag*, float minInclusive, float maxInclusive);

        static std::string GetActivePack(RE::StaticFunctionTag*);

        static void SetActivePack(RE::StaticFunctionTag*, std::string name);

        static std::vector<std::string> GetPacks(RE::StaticFunctionTag*);
    };
}