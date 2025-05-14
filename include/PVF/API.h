#pragma once

#define API_TYPE_KEY static_cast<uint32_t>(0xed01a811)

namespace PVF {
    struct DataValue {
        enum Type : int {
            kNone = 0,
            kString = 1,
            kInt = 2,
            kFloat = 3,
            kBool = 4,
            kForm = 5
        };

        Type type;
        union {
            std::string stringValue;
            int intValue;
            float floatValue;
            bool boolValue;
            RE::TESForm* formValue;
            void* noneValue;
        };

        DataValue() : type(kNone), noneValue(nullptr) {
        }
        explicit DataValue(const char* value) : type(kString), stringValue(value) {
        }
        explicit DataValue(const int value) : type(kInt), intValue(value) {
        }
        explicit DataValue(const float value) : type(kFloat), floatValue(value) {
        }
        explicit DataValue(const bool value) : type(kBool), boolValue(value) {
        }
        explicit DataValue(RE::TESForm* value) : type(kForm), formValue(value) {
        }

        DataValue(const DataValue& other) : type(other.type) {
            switch (type) {
                case kString:
                    new (&stringValue) std::string(other.stringValue);
                    break;
                case kInt:
                    intValue = other.intValue;
                    break;
                case kFloat:
                    floatValue = other.floatValue;
                    break;
                case kBool:
                    boolValue = other.boolValue;
                    break;
                case kForm:
                    formValue = other.formValue;
                    break;
                default:
                    noneValue = nullptr;
                    break;
            }
        }

        DataValue& operator=(const DataValue& other) {
            if (this != &other) {
                this->~DataValue();
                type = other.type;
                switch (type) {
                    case kString:
                        new (&stringValue) std::string(other.stringValue);
                        break;
                    case kInt:
                        intValue = other.intValue;
                        break;
                    case kFloat:
                        floatValue = other.floatValue;
                        break;
                    case kBool:
                        boolValue = other.boolValue;
                        break;
                    case kForm:
                        formValue = other.formValue;
                        break;
                    default:
                        noneValue = nullptr;
                        break;
                }
            }
            return *this;
        }

        ~DataValue() {
            if (type == kString) { stringValue.~basic_string(); }
        }

        Type GetType() const {
            return type;
        }

        std::string AsString() const {
            return type == kString ? stringValue : std::string("");
        }
        const char* AsCString() const {
            return type == kString ? stringValue.c_str() : "";
        }
        int AsInt() const {
            return type == kInt ? intValue : 0;
        }
        float AsFloat() const {
            return type == kFloat ? floatValue : 0.0f;
        }
        bool AsBool() const {
            return type == kBool ? boolValue : false;
        }
        RE::TESForm* AsForm() const {
            return type == kForm ? formValue : static_cast<RE::TESForm*>(nullptr);
        }

        void SetString(const char* val) {
            type = kString;
            stringValue = val;
        }
        void SetInt(const int val) {
            type = kInt;
            intValue = val;
        }
        void SetFloat(const float val) {
            type = kFloat;
            floatValue = val;
        }
        void SetBool(const bool val) {
            type = kBool;
            boolValue = val;
        }
        void SetForm(RE::TESForm* val) {
            type = kForm;
            formValue = val;
        }
    };

    typedef DataValue (*ConditionFunctionCallback)(void* context);
    typedef void (*ConditionFunctionCleanup)(void* context);

    struct ConditionFunction {
        ConditionFunctionCallback callback;
        void* context;
        ConditionFunctionCleanup cleanup;
    };

    struct IConditionFunctor {
        virtual ~IConditionFunctor() {
        }
        virtual DataValue call() = 0;
    };

    inline ConditionFunction CreateConditionFunction(IConditionFunctor* functor) {
        auto adapter = [](void* context) -> DataValue {
            const auto f = static_cast<IConditionFunctor*>(context);
            return f->call();
        };
        auto cleanupFunc = [](void* context) {
            const auto f = static_cast<IConditionFunctor*>(context);
            delete f;
        };
        return ConditionFunction{adapter, static_cast<void*>(functor), cleanupFunc};
    }

    template <typename F>
    IConditionFunctor* MakeConditionFunctor(F&& f) {
        struct FunctorImpl : IConditionFunctor {
            std::decay_t<F> func;
            explicit FunctorImpl(F&& funcIn) : func(std::forward<F>(funcIn)) {
            }
            DataValue call() override {
                return func();
            }
        };
        return new FunctorImpl(std::forward<F>(f));
    }

    template <typename F>
    ConditionFunction CreateConditionFunction(F&& f) {
        return CreateConditionFunction(MakeConditionFunctor(std::forward<F>(f)));
    }
}

namespace PVF_API {
    class PlayerVoiceFrameworkAPI {
    public:
        // Logs a message at various log-levels
        virtual void LogInfo(const char* message);
        virtual void LogWarn(const char* message);
        virtual void LogError(const char* message);
        virtual void LogCritical(const char* message);
        virtual void LogDebug(const char* message);

        // Sends a sound-event with the given name.
        // If a sound with that name isn't registered in a user's voice-pack, it'll be ignored.
        virtual void SendSoundEvent(const char* name);

        // Registers a custom condition for the given event.
        // For 'eventName', use the name of the event to attach it to.
        // Conditions attached to events will automatically unregister once the event is sent/played, so you should only send conditions right before sending the corresponding event.
        virtual void RegisterCondition(const char* eventName, const char* conditionName, const PVF::ConditionFunction& conditionFunction);

        // Registers a custom condition that is globally available.
        // They will never unregister, and be available from the moment you register them.
        virtual void RegisterGlobalCondition(const char* conditionName, const PVF::ConditionFunction& conditionFunction);

        // Returns whether or not the given form contains the given keyword.
        virtual bool FormHasKeyword(RE::TESForm* form, const char* keyword);

        // Returns a random integer between 'minInclusive' and 'maxInclusive'
        virtual int RandomInt(int minInclusive, int maxInclusive);

        // Returns a random float between 'minInclusive' and 'maxInclusive'
        virtual float RandomFloat(float minInclusive, float maxInclusive);
    };

    inline extern PlayerVoiceFrameworkAPI* ptr = nullptr;

    // Use this to get the reference to the API
    inline extern PlayerVoiceFrameworkAPI* GetAPI() {
        if (ptr == nullptr) {
            // Try to Load API
            SKSE::GetMessagingInterface()->Dispatch(API_TYPE_KEY, &ptr, sizeof(void*), NULL);
        }
        return ptr;
    }
}