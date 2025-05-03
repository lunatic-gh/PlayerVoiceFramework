#pragma once

#include "API.h"
#include "Util.h"

namespace PVE {

    class ConditionManager {
        using Value = DataType;

    public:
        // Registers the internal conditions that come with this mod.
        void RegisterInternalConditions() {
            this->RegisterGlobalCondition("PlayerHealthPercentage", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                    const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
                    if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth); f2 != 0.0f) {
                        const float f = (f1 / f2) * 100.0f;
                        return DataType(f);
                    }
                }
                return DataType(0.0f);
            });

            this->RegisterGlobalCondition("PlayerStaminaPercentage", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                    const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
                    if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kStamina); f2 != 0.0f) {
                        const float f = (f1 / f2) * 100.0f;
                        return DataType(f);
                    }
                }
                return DataType(0.0f);
            });

            this->RegisterGlobalCondition("PlayerMagickaPercentage", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                    const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
                    if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kMagicka); f2 != 0.0f) {
                        const float f = (f1 / f2) * 100.0f;
                        return DataType(f);
                    }
                }
                return DataType(0.0f);
            });

            this->RegisterGlobalCondition("PlayerEquippedWeaponTypeLeft", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                if (const auto form = actor->GetEquippedObject(true)) {
                    if (const auto weapon = form->As<RE::TESObjectWEAP>()) {
                        return DataType(static_cast<int>(weapon->GetWeaponType()));
                    }
                }
                return DataType(std::numeric_limits<int>::lowest());
            });

            this->RegisterGlobalCondition("PlayerEquippedWeaponTypeRight", [] {
                if (const auto actor = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto form = actor->GetEquippedObject(false)) {
                        if (const auto weapon = form->As<RE::TESObjectWEAP>()) {
                            return DataType(static_cast<int>(weapon->GetWeaponType()));
                        }
                    }
                }
                return DataType(std::numeric_limits<int>::lowest());
            });

            this->RegisterGlobalCondition("PlayerName", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return DataType(player->GetName());
                }
                return DataType("");
            });

            this->RegisterGlobalCondition("PlayerRaceForm", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    // Assuming GetRace() returns a pointer convertible to RE::TESForm*
                    return DataType(static_cast<RE::TESForm*>(player->GetRace()));
                }
                return DataType(static_cast<RE::TESForm*>(nullptr));
            });

            this->RegisterGlobalCondition("PlayerSex", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto actorBase = player->GetActorBase()) {
                        return DataType(static_cast<int>(actorBase->GetSex()));
                    }
                }
                return DataType(std::numeric_limits<int>::lowest());
            });

            this->RegisterGlobalCondition("PlayerPosX", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return DataType(player->GetPositionX());
                }
                return DataType(std::numeric_limits<float>::lowest());
            });

            this->RegisterGlobalCondition("PlayerPosY", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return DataType(player->GetPositionY());
                }
                return DataType(std::numeric_limits<float>::lowest());
            });

            this->RegisterGlobalCondition("PlayerPosZ", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return DataType(player->GetPositionZ());
                }
                return DataType(std::numeric_limits<float>::lowest());
            });

            this->RegisterGlobalCondition("PlayerLocationForm", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto loc = player->GetCurrentLocation()) {
                        return DataType(static_cast<RE::TESForm*>(loc));
                    }
                }
                return DataType(static_cast<RE::TESForm*>(nullptr));
            });

            this->RegisterGlobalCondition("PlayerLocationKeywords", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto loc = player->GetCurrentLocation()) {
                        // Assuming FormUtil::ToKeywordString returns a std::string.
                        return DataType(FormUtil::ToKeywordString(loc).c_str());
                    }
                }
                return DataType("");
            });

            this->RegisterGlobalCondition("RandomInt", [] { return DataType(Util::RandomInt(1, 99)); });
            this->RegisterGlobalCondition("RandomFloat", [] { return DataType(Util::RandomFloat(1.0f, 99.0f)); });
            this->RegisterGlobalCondition("RandomBool", [] { return DataType(Util::RandomInt(1, 99) == 1); });

            this->RegisterGlobalCondition("PlayerWerewolfState", [] {
                if (const auto globalVar = RE::TESForm::LookupByID(0xed06c)->As<RE::TESGlobal>()) {
                    if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                        if (globalVar->value == 1.0f) {
                            if (player->GetRace()->GetLocalFormID() == 0xCDD84) {
                                return DataType(2);
                            }
                            return DataType(1);
                        }
                    }
                }
                return DataType(0);
            });

            this->RegisterGlobalCondition("PlayerVampireState", [] {
                if (const auto globalVar = RE::TESForm::LookupByID(0xed06D)->As<RE::TESGlobal>()) {
                    if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                        if (globalVar->value == 1.0f) {
                            if (player->GetRace()->GetLocalFormID() == 0x283A) {
                                return DataType(2);
                            }
                            return DataType(1);
                        }
                    }
                }
                return DataType(0);
            });
        }

        static ConditionManager* GetSingleton() {
            static ConditionManager instance;
            return &instance;
        }

        ConditionManager(const ConditionManager&) = delete;
        ConditionManager& operator=(const ConditionManager&) = delete;

        /**
         * Registers a condition for the given event (or globally).
         *
         * @param eventName the name of the event to attach, or "GLOBAL" for a global condition.
         * @param conditionName the name of the condition.
         * @param conditionFunction a function whose return value (a VariantType) is used for evaluation.
         */
        void RegisterCondition(const std::string& eventName, const std::string& conditionName, const std::function<DataType()>& conditionFunction) {
            auto& conditionList = conditions[eventName];
            // Replace an existing condition if one exists.
            std::ranges::replace_if(conditionList, [&](const Condition& condition) { return condition.name == conditionName; }, Condition{conditionName, conditionFunction});
            // Add the condition if it did not exist.
            if (!std::ranges::any_of(conditionList, [&](const Condition& condition) { return condition.name == conditionName; })) {
                conditionList.emplace_back(Condition{conditionName, conditionFunction});
            }
        }

        /**
         * Convenience function to register a global condition.
         */
        void RegisterGlobalCondition(const std::string& conditionName, const std::function<DataType()>& conditionFunction) {
            RegisterCondition("GLOBAL", conditionName, conditionFunction);
        }

        /**
         * Unregisters all conditions for the given event.
         * (Global conditions are not removed.)
         *
         * @param eventName the name of the event.
         */
        void UnregisterConditions(const std::string& eventName) {
            if (eventName == "GLOBAL") {
                return;
            }
            conditions.erase(eventName);
        }

        /**
         * Evaluates the given expression for the given event.
         * @param eventName the name of the event
         * @param condition the expression to evaluate
         * @return true if the condition succeeds, false otherwise
         */
        bool EvaluateExpression(const std::string& eventName, const std::string& condition) const {
            if (condition.empty()) {
                return true;
            }
            const auto tokens = tokenize(condition);
            Parser parser(tokens);
            const std::unique_ptr<Expr> expr = parser.parseExpression();
            if (!expr) {
                return false;
            }
            return expr->eval(eventName);
        }

    private:
        ConditionManager() = default;
        enum class TokenType {
            Identifier,
            Float,
            Int,
            String,
            Bool,
            Operator,
            LeftParen,
            RightParen,
            End
        };

        struct Token {
            TokenType type;
            std::string value;
        };

        struct Condition {
            std::string name;
            std::function<DataType()> function;
        };

        std::vector<Token> tokenize(const std::string& str) const {
            std::vector<Token> tokens;
            size_t i = 0;
            while (i < str.size()) {
                if (std::isspace(str[i])) {
                    ++i;
                    continue;
                }
                if (i + 1 < str.size() && str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X')) {
                    size_t j = i + 2;
                    while (j < str.size() && std::isxdigit(str[j])) {
                        j++;
                    }
                    tokens.push_back({TokenType::Int, str.substr(i, j - i)});
                    i = j;
                    continue;
                }
                if (std::isdigit(str[i]) || (str[i] == '.' && i + 1 < str.size() && std::isdigit(str[i + 1]))) {
                    size_t j = i;
                    bool dotEncountered = false;
                    while (j < str.size() && (std::isdigit(str[j]) || str[j] == '.')) {
                        if (str[j] == '.') {
                            if (dotEncountered) break;
                            dotEncountered = true;
                        }
                        j++;
                    }
                    tokens.push_back({dotEncountered ? TokenType::Float : TokenType::Int, str.substr(i, j - i)});
                    i = j;
                    continue;
                }
                if (std::isalpha(str[i]) || str[i] == '_') {
                    size_t j = i;
                    while (j < str.size() && (std::isalnum(str[j]) || str[j] == '_')) {
                        j++;
                    }
                    if (std::string tokenStr = str.substr(i, j - i); tokenStr == "true" || tokenStr == "false") {
                        tokens.push_back({TokenType::Bool, tokenStr});
                    } else {
                        tokens.push_back({TokenType::Identifier, tokenStr});
                    }
                    i = j;
                    continue;
                }
                if (str[i] == '\'' || str[i] == '"') {
                    const char quote = str[i];
                    i++;
                    std::string literal;
                    while (i < str.size()) {
                        if (str[i] == quote) {
                            if (i + 1 < str.size() && str[i + 1] == quote) {
                                literal.push_back(quote);
                                i += 2;
                            } else {
                                i++;
                                break;
                            }
                        } else if (str[i] == '\\') {
                            if (i + 1 < str.size()) {
                                literal.push_back(str[i + 1]);
                                i += 2;
                            } else {
                                literal.push_back(str[i]);
                                i++;
                            }
                        } else {
                            literal.push_back(str[i]);
                            i++;
                        }
                    }
                    tokens.push_back({TokenType::String, literal});
                    continue;
                }
                if (str[i] == '(') {
                    tokens.push_back({TokenType::LeftParen, "("});
                    ++i;
                    continue;
                }
                if (str[i] == ')') {
                    tokens.push_back({TokenType::RightParen, ")"});
                    ++i;
                    continue;
                }
                if (i + 1 < str.size()) {
                    if (std::string two = str.substr(i, 2); two == "&&" || two == "||" || two == "==" || two == "!=" || two == "<=" || two == ">=" || two == "*=") {
                        tokens.push_back({TokenType::Operator, two});
                        i += 2;
                        continue;
                    }
                }
                if (str[i] == '<' || str[i] == '>') {
                    tokens.push_back({TokenType::Operator, std::string(1, str[i])});
                    ++i;
                    continue;
                }
                ++i;
            }
            tokens.push_back({TokenType::End, ""});
            return tokens;
        }

        class Expr {
        public:
            virtual ~Expr() = default;

            [[nodiscard]] virtual bool eval(const std::string& eventName) const = 0;
        };

        class LogicalExpr final : public Expr {
        public:
            enum class Op {
                And,
                Or
            };

            std::unique_ptr<Expr> left;
            std::unique_ptr<Expr> right;
            Op op;

            LogicalExpr(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r, const Op o) : left(std::move(l)), right(std::move(r)), op(o) {
            }

            bool eval(const std::string& eventName) const override {
                if (op == Op::And) return left->eval(eventName) && right->eval(eventName);
                else
                    return left->eval(eventName) || right->eval(eventName);
            }
        };

        class CondExpr final : public Expr {
        public:
            std::string var;
            std::string op;
            DataType literal;

            CondExpr(const std::string& v, const std::string& o, const DataType& lit) : var(v), op(o), literal(lit) {
            }

            bool eval(const std::string& eventName) const override {
                DataType val;
                auto searchConditions = [&](const std::string& key) -> std::optional<DataType> {
                    if (GetSingleton()->conditions.contains(key)) {
                        auto eventConditions = GetSingleton()->conditions[key];
                        if (const auto it = std::ranges::find_if(eventConditions, [&](const auto& cond) { return cond.name == var; }); it != eventConditions.end()) {
                            return it->function();
                        }
                    }
                    return std::nullopt;
                };
                if (const auto eventVal = searchConditions(eventName); eventVal.has_value()) {
                    val = eventVal.value();
                } else if (const auto globalVal = searchConditions("GLOBAL"); globalVal.has_value()) {
                    val = globalVal.value();
                } else {
                    return false;
                }

                if (val.GetType() == DataType::Type::kForm && (op == "==" || op == "!=")) {
                    const auto lhs = val.AsForm();
                    std::string rhs = "";
                    if (literal.GetType() == DataType::Type::kString) {
                        rhs = literal.AsString();
                    } else {
                        return false;
                    }
                    const bool result = FormUtil::CompareForms(lhs, rhs);
                    return op == "==" ? result : !result;
                }
                if (val.GetType() == DataType::Type::kFloat) {
                    float lhs = val.AsFloat();
                    float rhs = 0.0f;
                    if (literal.GetType() == DataType::Type::kFloat) {
                        rhs = literal.AsFloat();
                    } else if (literal.GetType() == DataType::Type::kInt) {
                        rhs = static_cast<float>(literal.AsInt());
                    } else {
                        return false;
                    }
                    Logger::GetSingleton().LogDebug(std::format("{} | {}", lhs, rhs));
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    if (op == "<") return lhs < rhs;
                    if (op == "<=") return lhs <= rhs;
                    if (op == ">") return lhs > rhs;
                    if (op == ">=") return lhs >= rhs;
                }
                // Compare int values.
                else if (val.GetType() == DataType::Type::kInt) {
                    int lhs = val.AsInt();
                    int rhs = 0;
                    if (literal.GetType() == DataType::Type::kInt) {
                        rhs = literal.AsInt();
                    } else if (literal.GetType() == DataType::Type::kFloat) {
                        rhs = static_cast<int>(literal.AsFloat());
                    } else {
                        return false;
                    }
                    Logger::GetSingleton().LogDebug(std::format("{} | {}", lhs, rhs));
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    if (op == "<") return lhs < rhs;
                    if (op == "<=") return lhs <= rhs;
                    if (op == ">") return lhs > rhs;
                    if (op == ">=") return lhs >= rhs;
                }
                // Compare bool values.
                else if (val.GetType() == DataType::Type::kBool) {
                    bool lhs = val.AsBool();
                    bool rhs = false;
                    if (literal.GetType() == DataType::Type::kBool) {
                        rhs = literal.AsBool();
                    } else {
                        return false;
                    }
                    Logger::GetSingleton().LogDebug(std::format("{} | {}", lhs, rhs));
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                } else if (val.GetType() == DataType::Type::kString) {
                    auto lhs = val.AsString();
                    std::string rhs = "";
                    if (literal.GetType() == DataType::Type::kString) {
                        rhs = literal.AsString();
                    } else {
                        return false;
                    }
                    Logger::GetSingleton().LogDebug(std::format("{} | {}", lhs, rhs));
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    if (op == "*=") return lhs.contains(rhs);
                    return false;
                }
                return false;
            }
        };

        class Parser {
        public:
            explicit Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {
            }

            std::unique_ptr<Expr> parseExpression() {
                return parseOr();
            }

        private:
            const std::vector<Token>& tokens;
            size_t pos;

            const Token& current() const {
                return tokens[pos];
            }

            void consume() {
                ++pos;
            }

            bool match(const TokenType type, const std::string& val = "") {
                if (current().type == type && (val.empty() || current().value == val)) {
                    consume();
                    return true;
                }
                return false;
            }

            std::unique_ptr<Expr> parseOr() {
                auto left = parseAnd();
                while (current().type == TokenType::Operator && current().value == "||") {
                    consume();
                    auto right = parseAnd();
                    left = std::make_unique<LogicalExpr>(std::move(left), std::move(right), LogicalExpr::Op::Or);
                }
                return left;
            }

            std::unique_ptr<Expr> parseAnd() {
                auto left = parseFactor();
                while (current().type == TokenType::Operator && current().value == "&&") {
                    consume();
                    auto right = parseFactor();
                    left = std::make_unique<LogicalExpr>(std::move(left), std::move(right), LogicalExpr::Op::And);
                }
                return left;
            }

            std::unique_ptr<Expr> parseFactor() {
                if (match(TokenType::LeftParen)) {
                    auto expr = parseExpression();
                    match(TokenType::RightParen);
                    return expr;
                }
                return parseCondition();
            }

            std::unique_ptr<Expr> parseCondition() {
                if (current().type != TokenType::Identifier) {
                    return nullptr;
                }
                std::string varName = current().value;
                consume();

                if (current().type != TokenType::Operator) {
                    return nullptr;
                }
                std::string op = current().value;
                consume();

                if (current().type == TokenType::Float) {
                    float num = std::stof(current().value);
                    consume();
                    Logger::GetSingleton().LogDebug("1");
                    Logger::GetSingleton().LogDebug(std::format("{}", num));
                    return std::make_unique<CondExpr>(varName, op, DataType(num));
                }
                if (current().type == TokenType::Int) {
                    int num = std::stoi(current().value, nullptr, current().value.starts_with("0x") ? 16 : 10);
                    consume();
                    Logger::GetSingleton().LogDebug("2");
                    Logger::GetSingleton().LogDebug(std::format("{}", num));
                    return std::make_unique<CondExpr>(varName, op, DataType(num));
                }
                if (current().type == TokenType::Bool) {
                    auto s = current().value;
                    std::ranges::transform(s, s.begin(), [](const unsigned char c) { return std::tolower(c); });
                    consume();
                    Logger::GetSingleton().LogDebug("3");
                    Logger::GetSingleton().LogDebug(std::format("{}", s));
                    return std::make_unique<CondExpr>(varName, op, DataType(s == "true"));
                }
                if (current().type == TokenType::String) {
                    auto str = current().value.c_str();
                    consume();
                    Logger::GetSingleton().LogDebug("4");
                    Logger::GetSingleton().LogDebug(std::format("{}", str));
                    return std::make_unique<CondExpr>(varName, op, DataType(str));
                }
                Logger::GetSingleton().LogDebug("5");
                Logger::GetSingleton().LogDebug(std::format("NULL"));
                return nullptr;
            }
        };

        std::unordered_map<std::string, std::vector<Condition>> conditions;
    };

}
