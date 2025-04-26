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
        // Sends a sound-event with the given name.
        // If a sound with that name isn't registered in a user's voice-pack, it'll be ignored.
        PVE_EXPORT static bool SendSoundEvent(const std::string& name);

        // 1.0.3: Added
        // Registers a custom condition
        // For 'eventName', use either 'GLOBAL' for registering a global condition, or the name of the event to attach it to.
        // Conditions attached to events will automatically unregister once the event is sent/played, so you should only send non-global events right before sending an event.
        PVE_EXPORT static void RegisterCondition(const std::string& eventName, const std::string& conditionName, const std::function<std::variant<float, int, bool, std::string, RE::TESForm*>()>& conditionFunction);

        // 1.0.3: Added
        // Sets a value into the Memory Data Storage.
        // As the name suggests, this storage is only in memory, and will be flushed once the game is closed.
        PVE_EXPORT static void SetMemoryData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value);

        // 1.0.3: Added
        // Retrieves a value from the Memory Data Storage. If the given key has no value, it returns 'def'
        // As the name suggests, this storage is only in memory, and will be flushed once the game is closed.
        PVE_EXPORT static std::variant<std::string, int, float, RE::TESForm*> GetMemoryData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def);

        // 1.0.3: Added
        // Sets a value into the Save Data Storage.
        // This Storage is linked to the currently loaded Playthrough, and will be saved/loaded whenever the game is saved/loaded.
        // Therefore, Anything saved before a save is loaded will be discarded once a save is loaded.
        PVE_EXPORT static void SetSaveData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& value);

        // 1.0.3: Added
        // Retrieves a value from the Save Data Storage. If the given key has no value, it returns 'def'
        // This Storage is linked to the currently loaded Playthrough, and will be saved/loaded whenever the game is saved/loaded.
        // Therefore, Anything saved before a save is loaded will be discarded once a save is loaded.
        PVE_EXPORT static std::variant<std::string, int, float, RE::TESForm*> GetSaveData(const std::string& key, const std::variant<std::string, int, float, RE::TESForm*>& def);

        // 1.0.3: Added
        // Returns whether or not the given form contains the given keyword.
        PVE_EXPORT static bool FormHasKeyword(RE::TESForm* form, const std::string& keyword);

        // 1.0.3: Added
        // Returns the given form as a string-representation in the format 'PluginName|HexFormID', e.g. 'Skyrim.esm|0x123456'
        PVE_EXPORT static std::string FormToString(const RE::TESForm* form);

        // 1.0.3: Added
        // Returns the form associated to the given form string. The string must be in the format 'PluginName|HexFormID', e.g. 'Skyrim.esm|0x123456'
        PVE_EXPORT static RE::TESForm* FormFromString(const std::string& formString);

        // 1.0.3: Added
        // Returns the form associated to the given plugin name and Form ID.
        PVE_EXPORT static RE::TESForm* FormFromID(const std::string& pluginName, const RE::FormID& formId);

        // 1.0.3: Added
        // Returns whether or not the two given forms are the same.
        PVE_EXPORT static bool CompareForms(const std::string& first, const std::string& second);

        // 1.0.3: Added
        // Returns whether or not the two given forms are the same. 'second' must be in the format 'PluginName|HexFormID', e.g. 'Skyrim.esm|0x123456'
        PVE_EXPORT static bool CompareForms(const RE::TESForm* first, const std::string& second);

        // 1.0.3: Added
        // Returns the given form's attached keywords as string, in the format 'keyword1|keyword2|...'
        PVE_EXPORT static std::string FormToKeywordString(RE::TESForm* form);

        // 1.0.3: Added
        // Returns the given string, with all occurences of 'oldSeq' replaced with 'newSeq'
        PVE_EXPORT static std::string ReplaceInString(const std::string& text, const std::string& oldSeq, const std::string& newSeq);

        // 1.0.3: Added
        // Returns the given string with all leading and trailing whitespaces removed
        PVE_EXPORT static std::string TrimString(const std::string& text);

        // 1.0.3: Added
        // Returns a vector that contains all parts of the given string split by the given delimiter char
        PVE_EXPORT static std::vector<std::string> SplitString(const std::string& text, const char& delimiter);

        // 1.0.3: Added
        // Returns a random integer between 'minInclusive' and 'maxInclusive'
        PVE_EXPORT static int RandomInt(int minInclusive, int maxInclusive);

        // 1.0.3: Added
        // Returns a random float between 'minInclusive' and 'maxInclusive'
        PVE_EXPORT static float RandomFloat(float minInclusive, float maxInclusive);
    };
}