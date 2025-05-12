#pragma once

#define API_TYPE_KEY static_cast<uint32_t>(0xed01a811)

namespace PVF {
    using Variant = std::variant<std::string, int, float, bool, RE::TESForm*>;

    class DataValue {
    public:
        enum class Type : int {
            kString = 0,
            kInt = 1,
            kFloat = 2,
            kBool = 3,
            kForm = 4,
            kNull = -1
        };

        DataValue() = default;

        explicit DataValue(const char* val) {
            type = Type::kString;
            data = std::string(val);
        }

        explicit DataValue(int val) {
            type = Type::kInt;
            data = val;
        }

        explicit DataValue(float val) {
            type = Type::kFloat;
            data = val;
        }

        explicit DataValue(bool val) {
            type = Type::kBool;
            data = val;
        }

        explicit DataValue(RE::TESForm* val) {
            type = Type::kForm;
            data = val;
        }

        Type GetType() const {
            return type;
        };

        std::string AsString() const {
            return type == Type::kString ? std::get<std::string>(data) : "";
        }

        const char* AsCString() const {
            return type == Type::kString ? std::get<std::string>(data).c_str() : "";
        }

        int AsInt() const {
            return type == Type::kInt ? std::get<int>(data) : 0;
        }

        float AsFloat() const {
            return type == Type::kFloat ? std::get<float>(data) : 0.0f;
        }

        bool AsBool() const {
            return type == Type::kBool ? std::get<bool>(data) : false;
        }

        RE::TESForm* AsForm() const {
            return type == Type::kForm ? std::get<RE::TESForm*>(data) : static_cast<RE::TESForm*>(nullptr);
        }

        void SetString(const char* val) {
            this->type = Type::kString;
            data = std::string(val);
        }

        void SetInt(const int val) {
            this->type = Type::kInt;
            data = val;
        }

        void SetFloat(float val) {
            this->type = Type::kFloat;
            data = val;
        }

        void SetBool(bool val) {
            this->type = Type::kBool;
            data = val;
        }

        void SetForm(RE::TESForm* val) {
            this->type = Type::kFloat;
            data = val;
        }

    private:
        Type type;
        Variant data;
    };
}

namespace PVF_API {
    class ConditionFunction {
        using Function = PVF::DataValue (*)();

    public:
        Function function;
        explicit ConditionFunction(const Function fnct) : function(fnct) {};
    };

    class PlayerVoiceFrameworkAPI {
    public:
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

        // Returns whether or not the given form contains the given keyword.
        virtual bool FormHasKeyword(RE::TESForm* form, const char* keyword);

        // Returns a random integer between 'minInclusive' and 'maxInclusive'
        virtual int RandomInt(int minInclusive, int maxInclusive);

        // Returns a random float between 'minInclusive' and 'maxInclusive'
        virtual float RandomFloat(float minInclusive, float maxInclusive);
    };

    // USE 'GetAPI' instead!
    inline extern PlayerVoiceFrameworkAPI* ptr = nullptr;

    inline extern PlayerVoiceFrameworkAPI* GetAPI() {
        if (ptr == nullptr) {
            // Try to Load API
            SKSE::GetMessagingInterface()->Dispatch(API_TYPE_KEY, &ptr, sizeof(void*), NULL);
        }
        return ptr;
    }
}