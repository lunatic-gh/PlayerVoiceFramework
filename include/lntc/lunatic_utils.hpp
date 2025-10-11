#pragma once

#include <ranges>
#include <sstream>
#include <random>

/**
 * These are utilities for all kinds of cases.
 * If you want, you can use this header in your projects without any restrictions, i don't mind.
 */
namespace lntc {
  inline void str_to_lower(std::string& str) {
    std::ranges::transform(str, str.begin(), [](const unsigned char c) { return std::tolower(c); });
  }

  inline std::string str_to_lower_cpy(const std::string& str) {
    std::string str_lower = str;
    str_to_lower(str_lower);
    return str_lower;
  }

  inline void str_to_upper(std::string& str) {
    std::ranges::transform(str, str.begin(), [](const unsigned char c) { return std::toupper(c); });
  }

  inline std::string str_to_upper_cpy(const std::string& str) {
    std::string str_upper = str;
    str_to_upper(str_upper);
    return str_upper;
  }

  inline std::vector<std::string> str_split(const std::string& str, const char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
      result.push_back(item);
    }
    return result;
  }

  inline void str_replace_all(std::string& str, const std::string& old_seq, const std::string& new_seq) {
    if (old_seq.empty()) return;
    size_t pos = 0;
    while ((pos = str.find(old_seq, pos)) != std::string::npos) {
      str.replace(pos, old_seq.length(), new_seq);
      pos += new_seq.length();
    }
  }

  inline std::string str_replace_all_cpy(const std::string& str, const std::string& old_seq, const std::string& new_seq) {
    std::string result = str;
    str_replace_all(result, old_seq, new_seq);
    return result;
  }

  inline bool str_equals(const std::string& str_left, const std::string& str_right) { return str_left == str_right; }

  inline bool str_equals_ci(const std::string& str_left, const std::string& str_right) { return str_equals(str_to_lower_cpy(str_left), str_to_lower_cpy(str_right)); }

  inline bool str_starts_with(const std::string& str_left, const std::string& str_right) { return str_left.starts_with(str_right); }

  inline bool str_starts_with_ci(const std::string& str_left, const std::string& str_right) { return str_starts_with(str_to_lower_cpy(str_left), str_to_lower_cpy(str_right)); }

  inline bool str_ends_with(const std::string& str_left, const std::string& str_right) { return str_left.ends_with(str_right); }

  inline bool str_ends_with_ci(const std::string& str_left, const std::string& str_right) { return str_ends_with(str_to_lower_cpy(str_left), str_to_lower_cpy(str_right)); }

  inline bool str_contains(const std::string& str_left, const std::string& str_right) { return str_left.contains(str_right); }

  inline bool str_contains_ci(const std::string& str_left, const std::string& str_right) { return str_contains(str_to_lower_cpy(str_left), str_to_lower_cpy(str_right)); }

  template <class T = int>
  T random_int(const T min = std::numeric_limits<T>::min(), const T max = std::numeric_limits<T>::max()) {
    std::random_device random_dev;
    std::mt19937 gen(random_dev());
    std::uniform_int_distribution<T> dist(min, max);
    return dist(gen);
  }

  template <class T>
  class Singleton {
  public:
    static T* get_singleton() {
      static T singleton;
      return std::addressof(singleton);
    }

  protected:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
  };

#ifdef __CMAKE_COMMONLIBSSE_PLUGIN
  // Utilities for CommonLibSSE-NG (Skyrim SKSE Modding)
  namespace clib {
    template <class T>
    T* load_form(const RE::FormID& local_form_id, const std::string& mod_name) {
      const auto data_handler = RE::TESDataHandler::GetSingleton();
      if (!data_handler) {
        return nullptr;
      }

      const auto mod_info = data_handler->LookupModByName(mod_name);
      if (!mod_info) {
        return nullptr;
      }

      RE::FormID form_id;
      if (mod_info->IsLight()) {
        form_id = 0xFE000000 | (mod_info->GetPartialIndex() << 12) | (local_form_id & 0xFFF);
      } else {
        form_id = (mod_info->GetCompileIndex() << 24) | (local_form_id & 0x00FFFFFF);
      }
      auto form = RE::TESForm::LookupByID(form_id);
      return form ? form->As<T>() : static_cast<T*>(nullptr);
    }

    inline std::string keywords_to_string(const std::span<RE::BGSKeyword*>& keywords) {
      if (keywords.empty()) return "";
      std::string s = ";";
      for (const auto keyword : keywords) {
        if (keyword == keywords.back()) {
          s += keyword->GetFormEditorID();
          s += ";";
        }
      }
      return s;
    }

    inline std::string keywords_to_string(const std::vector<RE::BGSKeyword*>& keywords) {
      if (keywords.empty()) return "";
      std::string s = ";";
      for (const auto keyword : keywords) {
        if (keyword == keywords.back()) {
          s += keyword->GetFormEditorID();
          s += ";";
        }
      }
      return s;
    }
  }
#endif
}