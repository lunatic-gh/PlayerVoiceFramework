#pragma once

#ifdef PVE_EXPORTS
#define PVE_EXPORT __declspec(dllexport)
#else
#define PVE_EXPORT __declspec(dllimport)
#endif

namespace PVE {
    class API {
    public:
        // 1.0.3: Added
        PVE_EXPORT static bool SendSoundEvent(const std::string& name);

        // 1.0.3: Added
        PVE_EXPORT static void RegisterCondition(const std::string& eventName, const std::string& conditionName, const std::function<std::variant<float, int, bool, std::string, RE::TESForm*>()>& conditionFunction);

        // 1.0.3: Added
        PVE_EXPORT static void SetMemoryData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value);

        // 1.0.3: Added
        PVE_EXPORT static std::variant<std::string, int, float, RE::TESForm*> GetMemoryData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def);

        // 1.0.3: Added
        PVE_EXPORT static void SetSaveData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value);

        // 1.0.3: Added
        PVE_EXPORT static std::variant<std::string, int, float, RE::TESForm*> GetSaveData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def);

        // 1.0.3: Added
        PVE_EXPORT static bool FormHasKeyword(RE::TESForm* form, const std::string& keyword);

        // 1.0.3: Added
        PVE_EXPORT static std::string FormToString(const RE::TESForm* form);

        // 1.0.3: Added
        PVE_EXPORT static RE::TESForm* FormFromString(const std::string& formString);

        // 1.0.3: Added
        PVE_EXPORT static RE::TESForm* FormFromID(const std::string& pluginName, const RE::FormID& formId);

        // 1.0.3: Added
        PVE_EXPORT static bool CompareForms(const std::string& first, const std::string& second);

        // 1.0.3: Added
        PVE_EXPORT static bool CompareForms(const RE::TESForm* first, const std::string& second);

        // 1.0.3: Added
        PVE_EXPORT static std::string FormToKeywordString(RE::TESForm* form);

        // 1.0.3: Added
        PVE_EXPORT static std::string ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq);

        // 1.0.3: Added
        PVE_EXPORT static std::string TrimString(const std::string& text);

        // 1.0.3: Added
        PVE_EXPORT static std::vector<std::string> SplitString(const std::string& text, const char& delimiter);

        // 1.0.3: Added
        PVE_EXPORT static int RandomInt(int minInclusive, int maxInclusive);

        // 1.0.3: Added
        PVE_EXPORT static float RandomFloat(float minInclusive, float maxInclusive);
    };
}