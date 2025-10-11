#pragma once

#include <pvf/pvf_api.hpp>

#include <string>
#include <vector>

namespace pvf {
  inline bool eval_condition(const std::tuple<std::string, std::string, std::string>& condition, const std::vector<SoundEvent::SoundEventVariable>& variables) {
    try {
      const auto condition_var_name = std::get<0>(condition);
      const auto condition_var_op = std::get<1>(condition);
      const auto condition_var_value = std::get<2>(condition);

      auto find_var = [&](const std::string& name) -> const SoundEvent::SoundEventVariable* {
        for (const auto& var : variables) {
          if (lntc::str_equals_ci(var.name(), name)) return &var;
        }
        return nullptr;
      };

      auto compare = [](const SoundEvent::SoundEventVariable& left, const std::string& right, std::string op) {
        lntc::str_to_lower(op);
        switch (left.type()) {
          case SoundEvent::SoundEventVariable::Type::STRING: {
            if (op == "equal_to") return std::string(left.as_str()) == right;
            if (op == "not_equal_to") return std::string(left.as_str()) != right;
            if (op == "contains") return std::string(left.as_str()).contains(right);
            if (op == "not_contains") return !std::string(left.as_str()).contains(right);
            break;
          }
          case SoundEvent::SoundEventVariable::Type::INT:
          case SoundEvent::SoundEventVariable::Type::DOUBLE: {
            const double dbl_left = left.type() == SoundEvent::SoundEventVariable::Type::DOUBLE ? left.as_double() : left.as_int();
            const double dbl_right = right.contains(".") ? std::stod(right) : std::stoi(right, nullptr, right.starts_with("0x") ? 16 : 10);
            if (op == "equal_to") return dbl_left == dbl_right;
            if (op == "not_equal_to") return dbl_left != dbl_right;
            if (op == "less_than") return dbl_left < dbl_right;
            if (op == "less_than_or_equal_to") return dbl_left <= dbl_right;
            if (op == "greater_than") return dbl_left > dbl_right;
            if (op == "greater_than_or_equal_to") return dbl_left >= dbl_right;
            break;
          }
          case SoundEvent::SoundEventVariable::Type::BOOL: {
            const bool b_left = left.as_bool();
            bool b_right = false;
            if (lntc::str_equals_ci(right, "true")) {
              b_right = true;
            } else if (!lntc::str_equals_ci(right, "false")) {
              return false;
            }
            if (op == "equal_to") return b_left == b_right;
            if (op == "not_equal_to") return b_left != b_right;
            break;
          }
        }
        return false;
      };

      const SoundEvent::SoundEventVariable* var = find_var(condition_var_name);
      if (!var) return false;
      return compare(*var, condition_var_value, condition_var_op);
    } catch (const std::exception& e) {
    }
    return false;
  }

  inline double get_wav_duration(const fs::path& path) {
    if (fs::is_regular_file(path) && path.extension().string().ends_with(".wav")) {
      try {
        if (std::ifstream in(path.string(), std::ios::binary); in.is_open()) {
          in.seekg(22);
          uint16_t channels;
          in.read(reinterpret_cast<char*>(&channels), sizeof(channels));
          in.seekg(24);
          uint32_t sample_rate;
          in.read(reinterpret_cast<char*>(&sample_rate), sizeof(sample_rate));
          in.seekg(40);
          uint32_t date_size;
          in.read(reinterpret_cast<char*>(&date_size), sizeof(date_size));
          uint16_t bits;
          in.seekg(34);
          in.read(reinterpret_cast<char*>(&bits), sizeof(bits));
          const uint32_t bytes = bits / 8 * channels;
          return static_cast<double>(date_size) / (sample_rate * bytes) * 1000;
        }
      } catch (...) {
      }
    }
    return 0;
  }
}