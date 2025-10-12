// ReSharper disable CppPolymorphicClassWithNonVirtualPublicDestructor
#pragma once

#define PVF_API_KEY static_cast<uint32_t>(0x707666)
#define PVF_API_VERSION static_cast<uint32_t>(1)

namespace pvf_api {
  class SoundEvent {
  public:
    class Variable {
    public:
      enum class Type : uint32_t { STRING = 0, INT = 1, FLOAT = 2, BOOL = 3 };

      /// WARNINGS ABOUT MEMORY BEING LEAKED ARE FALSE-WARNINGS.
      /// DO NOT DELETE THE POINTER, THE FRAMEWORK WILL DO THAT AFTER THE EVENT IS PLAYED/DISCARDED! <br>
      /// Example usage: <br>
      /// @code const auto event = new pvf_api::SoundEvent("MyEventName"); @endcode <br>
      /// @code event->add_variable(new pvf_api::SoundEvent::Variable("MyVar", "MyValue")); @endcode
      Variable(const RE::BSString& name, const char* value) : type_(Type::STRING), name_(name), value_({value}) {}
      Variable(const RE::BSString& name, const uint32_t value) : type_(Type::INT), name_(name), value_({std::to_string(value)}) {}
      Variable(const RE::BSString& name, const float value) : type_(Type::FLOAT), name_(name), value_({std::to_string(value)}) {}
      Variable(const RE::BSString& name, const bool value) : type_(Type::BOOL), name_(name), value_({std::to_string(value)}) {}
      Variable(const RE::BSString& name, const RE::TESForm* value) : type_(Type::INT), name_(name), value_({std::to_string(value ? value->GetLocalFormID() : 0)}) {}

      ~Variable() = default;

      [[nodiscard]] Type type() const { return type_; }

      [[nodiscard]] RE::BSString name() const { return name_; }

      [[nodiscard]] RE::BSString as_str() const { return value_; }

      [[nodiscard]] uint32_t as_int() const {
        if (this->type_ == Type::INT) return std::stoi(this->value_.c_str());
        if (this->type_ == Type::FLOAT) return static_cast<uint32_t>(std::stof(this->value_.c_str()));
        return 0;
      }

      [[nodiscard]] float as_float() const {
        if (this->type_ == Type::FLOAT || this->type_ == Type::INT) {
          return std::stof(this->value_.c_str());
        }
        return 0.0f;
      }

      [[nodiscard]] bool as_bool() const { return this->type_ == Type::BOOL && strcmp(this->value_.c_str(), "true") == 0; }

    private:
      Type type_;
      RE::BSString name_;
      RE::BSString value_;
    };

    /// WARNINGS ABOUT MEMORY BEING LEAKED ARE FALSE-WARNINGS.
    /// DO NOT DELETE THE POINTER, THE FRAMEWORK WILL DO THAT AFTER THE EVENT IS PLAYED/DISCARDED! <br>
    /// Example usage: <br>
    /// @code const auto event = new pvf_api::SoundEvent("MyEventName"); @endcode
    SoundEvent(const char* name) : name_({name}) {}

    ~SoundEvent() = default;

    [[nodiscard]] RE::BSString name() const { return name_; }

    [[nodiscard]] std::vector<Variable*> variables() const { return variables_; }

    void add_variable(Variable* variable) { this->variables_.emplace_back(variable); }

  private:
    RE::BSString name_;
    std::vector<Variable*> variables_ = {};
  };

  class PlayerVoiceFrameworkAPI {
  public:
    virtual uint32_t api_version() const;

    virtual void send_sound_event(const SoundEvent* event);
  };

  inline extern PlayerVoiceFrameworkAPI* pvf_api = nullptr;

  // Attempts to load the API.
  inline PlayerVoiceFrameworkAPI* load_api() {
    if (pvf_api == nullptr) {
      SKSE::GetMessagingInterface()->Dispatch(PVF_API_KEY, &pvf_api, sizeof(void*), nullptr);
    }
    return pvf_api;
  }
}
