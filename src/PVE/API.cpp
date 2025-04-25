#include "../include/PVE/API.h"

#include "../../include/PVE/SoundManager.h"
#include "../../include/PVE/Util.h"

/**
 * NOTE TO MYSELF: DON'T CHANGE EXISTING DEFINITIONS UNLESS ABSOLUTELY NECESSARY!
 * RATHER DEPRECATE THEM.
 */
namespace PVE {

    bool API::SendSoundEvent(const std::string& name) {
        return SoundManager::GetSingleton().SendSoundEvent(name);
    }

    bool API::FormHasKeyword(RE::TESForm* form, const std::string& keyword) {
        return FormUtil::HasKeyword(form, keyword);
    }

    std::string API::FormToString(const RE::TESForm* form) {
        return FormUtil::ToString(form);
    }

    RE::TESForm* API::FormFromString(const std::string& formString) {
        return FormUtil::FromString(formString);
    }

    RE::TESForm* API::FormFromID(const std::string& pluginName, const RE::FormID& formId) {
        return FormUtil::FromID(pluginName, formId);
    }

    bool API::CompareForms(const std::string& first, const std::string& second) {
        return FormUtil::CompareForms(first, second);
    }

    bool API::CompareForms(const RE::TESForm* first, const std::string& second) {
        return FormUtil::CompareForms(first, second);
    }

    std::string API::FormToKeywordString(RE::TESForm* form) {
        return FormUtil::ToKeywordString(form);
    }

    std::string API::ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq) {
        return StringUtil::ReplaceInString(text, oldSeq, newSeq);
    }

    std::string API::TrimString(const std::string& text) {
        return StringUtil::TrimString(text);
    }

    std::vector<std::string> API::SplitString(const std::string& text, const char& delimiter) {
        return StringUtil::SplitString(text, delimiter);
    }

    int API::RandomInt(const int minInclusive, const int maxInclusive) {
        return Util::RandomInt(minInclusive, maxInclusive);
    }

    float API::RandomFloat(const float minInclusive, const float maxInclusive) {
        return Util::RandomFloat(minInclusive, maxInclusive);
    }
}