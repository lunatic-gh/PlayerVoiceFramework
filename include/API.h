#pragma once

#ifdef PVE_EXPORTS
#define PVE_EXPORT __declspec(dllexport)
#else
#define PVE_EXPORT __declspec(dllimport)
#endif

/**
 * NOTE TO MYSELF: DON'T CHANGE EXISTING DEFINITIONS UNLESS ABSOLUTELY NECESSARY!
 * RATHER DEPRECATE THEM.
 */
namespace PVE {
    class API {
    public:
        PVE_EXPORT static bool SendSoundEvent(const std::string& name);

        PVE_EXPORT static bool FormHasKeyword(RE::TESForm* form, const std::string& keyword);

        PVE_EXPORT static std::string FormToString(const RE::TESForm* form);

        PVE_EXPORT static RE::TESForm* FormFromString(const std::string& formString);

        PVE_EXPORT static RE::TESForm* FormFromID(const std::string& pluginName, const RE::FormID& formId);

        PVE_EXPORT static bool CompareForms(const std::string& first, const std::string& second);

        PVE_EXPORT static bool CompareForms(const RE::TESForm* first, const std::string& second);

        PVE_EXPORT static std::string FormToKeywordString(RE::TESForm* form);

        PVE_EXPORT static std::string ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq);

        PVE_EXPORT static std::string TrimString(const std::string& text);

        PVE_EXPORT static std::vector<std::string> SplitString(const std::string& text, const char& delimiter);

        PVE_EXPORT static int RandomInt(int minInclusive, int maxInclusive);

        PVE_EXPORT static float RandomFloat(float minInclusive, float maxInclusive);
    };
}