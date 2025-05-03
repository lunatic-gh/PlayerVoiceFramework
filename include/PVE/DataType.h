#pragma once

namespace PVE {
    class DataType {
    public:
        enum Type : int {
            kString = 0,
            kInt = 1,
            kFloat = 2,
            kBool = 3,
            kForm = 4,
            kNull = -1
        };

        DataType() : type(kNull), data(nullptr) {
        }

        explicit DataType(const char* val) : type(kString), data(std::string(val)) {
        }

        explicit DataType(const int val) : type(kInt), data(val) {
        }

        explicit DataType(const float val) : type(kFloat), data(val) {
        }

        explicit DataType(const bool val) : type(kBool), data(val) {
        }

        explicit DataType(RE::TESForm* val) : type(kForm), data(val) {
        }

        Type GetType() const {
            return type;
        }

        std::string AsString() const {
            return type == kString ? std::get<std::string>(data) : "";
        }

        void SetString(const char* str) {
            type = kString;
            data = std::string(str);
        }

        const char* AsCString() const {
            return type == kString ? std::get<std::string>(data).c_str() : "";
        }

        int AsInt() const {
            return type == kInt ? std::get<int>(data) : 0;
        }

        void SetInt(const int i) {
            type = kInt;
            data = i;
        }

        float AsFloat() const {
            return type == kFloat ? std::get<float>(data) : 0.0f;
        }

        void SetFloat(const float f) {
            type = kFloat;
            data = f;
        }

        bool AsBool() const {
            return type == kBool ? std::get<bool>(data) : false;
        }

        void SetBool(const bool b) {
            type = kBool;
            data = b;
        }

        RE::TESForm* AsForm() const {
            return type == kForm ? std::get<RE::TESForm*>(data) : static_cast<RE::TESForm*>(nullptr);
        }

        void SetForm(RE::TESForm* form) {
            type = kForm;
            data = form;
        }

    private:
        Type type;
        std::variant<std::string, int, float, bool, RE::TESForm*> data;
    };
}
