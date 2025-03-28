#pragma once

#include "Utils.h"

namespace PVE {
    class ConditionManager {
    public:
        /**
         * Internal Function for registering built-in functions.
         */
        static void Init() {
            RegisterCondition("GetPlayerHealthPercentage", [] {
                const auto actorValueOwner = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
                const float f1 = actorValueOwner->GetActorValue(RE::ActorValue::kHealth);
                const float f2 = actorValueOwner->GetPermanentActorValue(RE::ActorValue::kHealth);
                return std::to_string((f1 / f2) * 100.0f);
            });
            RegisterCondition("GetPlayerStaminaPercentage", [] {
                const auto actorValueOwner = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
                const float f1 = actorValueOwner->GetActorValue(RE::ActorValue::kStamina);
                const float f2 = actorValueOwner->GetPermanentActorValue(RE::ActorValue::kStamina);
                return std::to_string((f1 / f2) * 100.0f);
            });
            RegisterCondition("GetPlayerMagickaPercentage", [] {
                const auto actorValueOwner = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
                const float f1 = actorValueOwner->GetActorValue(RE::ActorValue::kMagicka);
                const float f2 = actorValueOwner->GetPermanentActorValue(RE::ActorValue::kMagicka);
                return std::to_string((f1 / f2) * 100.0f);
            });
            RegisterCondition("GetPlayerEquippedWeaponTypeLeft", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                if (actor->GetEquippedObject(true)->Is(RE::FormType::Weapon)) {
                    return std::to_string(actor->GetEquippedObject(true)->As<RE::TESObjectWEAP>()->GetWeaponType());
                }
                return std::string("-1");
            });
            RegisterCondition("GetPlayerEquippedWeaponTypeRight", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                if (actor->GetEquippedObject(false)->Is(RE::FormType::Weapon)) {
                    return std::to_string(actor->GetEquippedObject(false)->As<RE::TESObjectWEAP>()->GetWeaponType());
                }
                return std::string("-1");
            });
            RegisterCondition("GetPlayerRace", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                if (actor->GetRace()) {
                    return actor->GetRace()->GetName();
                }
                return "";
            });
            RegisterCondition("GetPlayerSex", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                if (actor->GetActorBase()) {
                    return std::to_string(actor->GetActorBase()->GetSex());
                }
                return std::string("-1");
            });
            RegisterCondition("GetPlayerName", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                return actor->GetName();
            });
        }

        /**
         * Registers the given condition for use. This is the intended way to register custom conditions.
         * See above how examples.
         * @param name the name of the function, which is used in the config.
         * @param fnct a lambda-function that returns the actual value as string-representation.
         */
        static void RegisterCondition(const std::string &name, std::function<std::string()> fnct) {
            conditions[name] = fnct;
        }

        /**
         * NOTE: Please don't use this. It is mainly here for debugging purposes...
         * @param name the name of the function, which is used in the config.
         * @param fnct a lambda-function that returns the actual value as string-representation.
         */
        static void UnregisterCondition(const std::string &name, std::function<std::string()> fnct) {
            conditions.erase(name);
        }

        /**
         * Evaluates the given condition statement.
         * @param expression the condition statement expression
         * @return the result of the expression.
         */
        static bool EvaluateConditions(std::string expression) {
            if (expression.empty()) {
                return true;
            }
            size_t openPos = expression.find_last_of('(');
            while (openPos != std::string::npos) {
                const size_t closePos = expression.find(')', openPos);
                if (closePos == std::string::npos) {
                    throw std::invalid_argument("Mismatched parentheses in expression.");
                }
                std::string innerExpr = expression.substr(openPos + 1, closePos - openPos - 1);
                const bool result = EvaluateCondition(innerExpr);
                expression.replace(openPos, closePos - openPos + 1, result ? "true" : "false");
                openPos = expression.find_last_of('(');
            }
            return EvaluateCondition(expression);
        }

    private:
        static bool EvaluateIntegers(const int i, const int i1, const std::string &op) {
            if (op == "==") return i == i1;
            if (op == "!=") return i != i1;
            if (op == "<") return i < i1;
            if (op == "<=") return i <= i1;
            if (op == ">") return i > i1;
            if (op == ">=") return i >= i1;
            return false;
        }

        static bool EvaluateFloats(const float f, const float f1, const std::string &op) {
            if (op == "==") return f == f1;
            if (op == "!=") return f != f1;
            if (op == "<") return f < f1;
            if (op == "<=") return f <= f1;
            if (op == ">") return f > f1;
            if (op == ">=") return f >= f1;
            return false;
        }

        static bool EvaluateStrings(const std::string &s, std::string &s1, const std::string &op) {
            const size_t start = s1.find_first_not_of('\'');
            const size_t end = s1.find_last_not_of('\'');
            if (start == std::string::npos || end == std::string::npos) {
                s1 = "";
            } else {
                s1 = s1.substr(start, end - start + 1);
            }
            if (op == "==") return strcmp(s.c_str(), s1.c_str()) == 0;
            if (op == "!=") return s != s1;
            if (op == "=*") return s.find(s1) != -1;
            return false;
        }

        static bool EvaluateValues(std::string value1, std::string value2, const std::string &op) {
            std::ranges::transform(value1, value1.begin(), [](const unsigned char c) { return std::tolower(c); });
            std::ranges::transform(value2, value2.begin(), [](const unsigned char c) { return std::tolower(c); });

            std::istringstream intStream(value1);
            std::istringstream intStream1(value2);
            if (int intValue, intValue1; intStream >> intValue && intStream1 >> intValue1 && intStream.eof() && intStream1.eof()) {
                return EvaluateIntegers(intValue, intValue1, op);
            }
            std::istringstream floatStream(value1);
            std::istringstream floatStream1(value2);
            if (float floatValue, floatValue1; floatStream >> floatValue && floatStream1 >> floatValue1 && floatStream.eof() && floatStream1.eof()) {
                return EvaluateFloats(floatValue, floatValue1, op);
            }
            if ((value1 == "true" && value2 == "true")) {
                return true;
            }
            if (value1 == "false" && value2 == "false") {
                return false;
            }
            return EvaluateStrings(value1, value2, op);
        }

        static bool EvaluateCondition(const std::string &expression) {
            std::string expr = expression;
            for (const auto &[fst, snd] : conditions) {
                size_t pos;
                while ((pos = expr.find(fst)) != std::string::npos) {
                    expr.replace(pos, fst.length(), snd());
                }
            }
            if (expr.find("&&") != std::string::npos) {
                const size_t pos = expr.find("&&");
                return EvaluateCondition(Utils::TrimString(expr.substr(0, pos))) && EvaluateCondition(Utils::TrimString(expr.substr(pos + 2)));
            }
            if (expr.find("||") != std::string::npos) {
                const size_t pos = expr.find("||");
                return EvaluateCondition(Utils::TrimString(expr.substr(0, pos))) || EvaluateCondition(Utils::TrimString(expr.substr(pos + 2)));
            }
            for (const auto ops = std::list<std::string>{"==", "!=", "=*", ">=", "<=", ">", "<"}; const auto &op : ops) {
                if (expr.find(op) != std::string::npos) {
                    const size_t pos = expr.find(op);
                    return EvaluateValues(Utils::TrimString(expr.substr(0, pos)), Utils::TrimString(expr.substr(pos + op.length())), op.c_str());
                }
            }
            return false;
        }

    private:
        inline static std::map<std::string, std::function<std::string()>> conditions;
    };
}