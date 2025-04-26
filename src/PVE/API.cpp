#include "../include/PVE/API.h"

#include "../../include/PVE/SoundManager.h"
#include "../../include/PVE/Util.h"

/**
 * NOTE TO MYSELF: DON'T CHANGE EXISTING DEFINITIONS UNLESS ABSOLUTELY NECESSARY!
 * RATHER DEPRECATE THEM.
 */
namespace PVE {
    // 1.0.3: Added
    bool API::SendSoundEvent(const std::string& name) {
        return SoundManager::GetSingleton().SendSoundEvent(name);
    }

    // 1.0.3: Added
    bool API::FormHasKeyword(RE::TESForm* form, const std::string& keyword) {
        return FormUtil::HasKeyword(form, keyword);
    }

    // 1.0.3: Added
    std::string API::FormToString(const RE::TESForm* form) {
        return FormUtil::ToString(form);
    }

    // 1.0.3: Added
    RE::TESForm* API::FormFromString(const std::string& formString) {
        return FormUtil::FromString(formString);
    }

    // 1.0.3: Added
    RE::TESForm* API::FormFromID(const std::string& pluginName, const RE::FormID& formId) {
        return FormUtil::FromID(pluginName, formId);
    }

    // 1.0.3: Added
    bool API::CompareForms(const std::string& first, const std::string& second) {
        return FormUtil::CompareForms(first, second);
    }

    // 1.0.3: Added
    bool API::CompareForms(const RE::TESForm* first, const std::string& second) {
        return FormUtil::CompareForms(first, second);
    }

    // 1.0.3: Added
    std::string API::FormToKeywordString(RE::TESForm* form) {
        return FormUtil::ToKeywordString(form);
    }

    // 1.0.3: Added
    std::string API::ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq) {
        return StringUtil::ReplaceInString(text, oldSeq, newSeq);
    }

    // 1.0.3: Added
    std::string API::TrimString(const std::string& text) {
        return StringUtil::TrimString(text);
    }

    // 1.0.3: Added
    std::vector<std::string> API::SplitString(const std::string& text, const char& delimiter) {
        return StringUtil::SplitString(text, delimiter);
    }

    // 1.0.3: Added
    int API::RandomInt(const int minInclusive, const int maxInclusive) {
        return Util::RandomInt(minInclusive, maxInclusive);
    }

    // 1.0.3: Added
    float API::RandomFloat(const float minInclusive, const float maxInclusive) {
        return Util::RandomFloat(minInclusive, maxInclusive);
    }
}