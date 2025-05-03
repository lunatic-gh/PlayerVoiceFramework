#pragma once

#define API_TYPE_KEY static_cast<uint32_t>(0xed01a811)

#define API_VERSION_MAJOR 1
#define API_VERSION_MINOR 0

#include "DataType.h"

namespace PVE_API {
    class ConditionFunction {
        using Function = PVE::DataType (*)();

    public:
        Function function;
        explicit ConditionFunction(const Function fnct) : function(fnct) {};
    };

    class PlayerVoiceEventsAPI {
    public:
        virtual int GetAPIVersionMajor();

        virtual int GetAPIVersionMinor();

        // Sends a sound-event with the given name.
        // If a sound with that name isn't registered in a user's voice-pack, it'll be ignored.
        virtual void SendSoundEvent(const char* name);

        // Registers a custom condition for the given event.
        // For 'eventName', use the name of the event to attach it to.
        // Conditions attached to events will automatically unregister once the event is sent/played, so you should only send conditions right before sending the corresponding event.
        virtual void RegisterCondition(const char* eventName, const char* conditionName, const ConditionFunction& conditionFunction);

        // Registers a custom condition that is globally available.
        // They will never unregister, and be available from the moment you register them.
        virtual void RegisterGlobalCondition(const char* conditionName, const ConditionFunction& conditionFunction);

        // Sets a value into the Memory Data Storage.
        // As the name suggests, this storage is only in memory, and will be flushed once the game is closed.
        virtual void SetMemoryData(const char* key, const PVE::DataType& value);

        // Retrieves a value from the Memory Data Storage. If the given key has no value, it returns 'def'
        // As the name suggests, this storage is only in memory, and will be flushed once the game is closed.
        virtual PVE::DataType GetMemoryData(const char* key, const PVE::DataType& def);

        // Sets a value into the Save Data Storage.
        // This Storage is linked to the currently loaded Playthrough, and will be saved/loaded whenever the game is saved/loaded.
        // Therefore, Anything saved before a save is loaded will be discarded once a save is loaded.
        virtual void SetSaveData(const char* key, const PVE::DataType& value);

        // Retrieves a value from the Save Data Storage. If the given key has no value, it returns 'def'
        // This Storage is linked to the currently loaded Playthrough, and will be saved/loaded whenever the game is saved/loaded.
        // Therefore, Anything saved before a save is loaded will be discarded once a save is loaded.
        virtual PVE::DataType GetSaveData(const char* key, PVE::DataType def);

        // Returns whether or not the given form contains the given keyword.
        virtual bool FormHasKeyword(RE::TESForm* form, const char* keyword);

        // Returns a random integer between 'minInclusive' and 'maxInclusive'
        virtual int RandomInt(int minInclusive, int maxInclusive);

        // Returns a random float between 'minInclusive' and 'maxInclusive'
        virtual float RandomFloat(float minInclusive, float maxInclusive);
    };

    inline extern PlayerVoiceEventsAPI* api_ptr = nullptr;

    inline extern PlayerVoiceEventsAPI* GetAPI() {
        return api_ptr;
    }

    inline bool LoadAPI() {
        if (api_ptr != nullptr) return true;
        SKSE::GetMessagingInterface()->Dispatch(API_TYPE_KEY, (void*)&api_ptr, sizeof(void*), NULL);
        if (api_ptr) {
            return API_VERSION_MAJOR == api_ptr->GetAPIVersionMajor() && API_VERSION_MINOR <= api_ptr->GetAPIVersionMinor();
        }
        return false;
    }
}