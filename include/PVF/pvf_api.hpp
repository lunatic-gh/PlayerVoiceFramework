#pragma once

#include <cstdint>
#include <memory>

#define PVF_API_KEY static_cast<uint32_t>(0x707666)
#define PVF_API_VERSION static_cast<uint32_t>(1)

namespace pvf {
  struct SoundEvent {
    struct SoundEventVariable {
      enum class Type : uint32_t { STRING = 0, INT = 1, DOUBLE = 2, BOOL = 3 };

      explicit SoundEventVariable(const char* name, const char* string_val) : name_(name), type_(Type::STRING), str_value_(string_val) {}
      explicit SoundEventVariable(const char* name, const uint32_t int_val) : name_(name), type_(Type::INT), int_value_(int_val) {}
      explicit SoundEventVariable(const char* name, const double double_val) : name_(name), type_(Type::DOUBLE), double_value_(double_val) {}
      explicit SoundEventVariable(const char* name, const bool bool_val) : name_(name), type_(Type::BOOL), bool_value_(bool_val) {}

      [[nodiscard]] const char* name() const { return name_; }

      [[nodiscard]] Type type() const { return type_; }

      [[nodiscard]] const char* as_str() const { return str_value_; }

      [[nodiscard]] uint32_t as_int() const { return int_value_; }

      [[nodiscard]] double as_double() const { return double_value_; }

      [[nodiscard]] bool as_bool() const { return bool_value_; }

    private:
      const char* name_;
      Type type_;
      union {
        const char* str_value_;
        uint32_t int_value_;
        double double_value_;
        bool bool_value_;
      };
    };

    SoundEvent(const char* name) : name_(name), variables_(nullptr), variables_len_(0), variables_capacity_(0) {}

    ~SoundEvent() {
      for (size_t i = 0; i < variables_len_; ++i) {
        variables_[i].~SoundEventVariable();
      }
      operator delete[](variables_);
    }

    void add_variable(const char* name, const char* value) { this->add_variable_(SoundEventVariable(name, value)); }
    void add_variable(const char* name, const uint32_t value) { this->add_variable_(SoundEventVariable(name, value)); }
    void add_variable(const char* name, const double value) { this->add_variable_(SoundEventVariable(name, value)); }
    void add_variable(const char* name, const bool value) { this->add_variable_(SoundEventVariable(name, value)); }
    void add_variable(const char* name, const RE::TESForm* value) { this->add_variable_(SoundEventVariable(name, value ? value->GetLocalFormID() : 0)); }

    const char* name() const { return name_; }

    const SoundEventVariable* variables() const { return variables_; }

    size_t variables_len() const { return variables_len_; }

    size_t variables_capacity() const { return variables_capacity_; }

  private:
    void add_variable_(const SoundEventVariable& variable) {
      if (!variable.name()) return;
      for (size_t i = 0; i < variables_len_; ++i) {
        if (variables_[i].name() == variable.name()) return;
      }
      if (variables_len_ >= variables_capacity_) {
        const size_t capacity = variables_capacity_ == 0 ? 4 : variables_capacity_ * 2;
        auto variables = static_cast<SoundEventVariable*>(operator new[](sizeof(SoundEventVariable) * capacity));
        for (size_t i = 0; i < variables_len_; ++i) {
          new (&variables[i]) SoundEventVariable(variables_[i]);
          variables_[i].~SoundEventVariable();
        }
        operator delete[](variables_);
        variables_ = variables;
        variables_capacity_ = capacity;
      }
      new (&variables_[variables_len_]) SoundEventVariable(variable);
      ++variables_len_;
    }

    const char* name_;
    SoundEventVariable* variables_;
    size_t variables_len_;
    size_t variables_capacity_;
  };

  // ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
  class PlayerVoiceFrameworkAPI {
  public:
    // Returns the API-Version from the framework installed on a user's system.
    // You can use this to compare it against your included header's "PVF_API_VERSION"
    virtual uint32_t get_api_version() const;

    // Sends the given sound event to the framework.
    virtual void send_sound_event(const SoundEvent& event);
  };

  inline extern PlayerVoiceFrameworkAPI* api = nullptr;

  // Attempts to load the API.
  inline bool load_api() {
    if (api == nullptr) {
      SKSE::GetMessagingInterface()->Dispatch(PVF_API_KEY, &api, sizeof(void*), nullptr);
    }
    return api != nullptr;
  }
}