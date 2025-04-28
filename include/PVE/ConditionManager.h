#pragma once

#include "Util.h"

namespace PVE {

    class ConditionManager {
        using Value = std::variant<float, int, bool, std::string, RE::TESForm*>;

    public:
        // Registers the internal conditions that come with this mod
        void RegisterInternalConditions() {
            this->RegisterGlobalCondition("PlayerHealthPercentage", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                    const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
                    if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth); f2 != 0.0f) {
                        const float f = (f1 / f2) * 100.0f;
                        return f;
                    }
                }
                return 0.0f;
            });
            this->RegisterGlobalCondition("PlayerStaminaPercentage", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                    const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
                    if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kStamina); f2 != 0.0f) {
                        const float f = (f1 / f2) * 100.0f;
                        return f;
                    }
                }
                return 0.0f;
            });
            this->RegisterGlobalCondition("PlayerMagickaPercentage", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                    const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
                    if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kMagicka); f2 != 0.0f) {
                        const float f = (f1 / f2) * 100.0f;
                        return f;
                    }
                }
                return 0.0f;
            });
            this->RegisterGlobalCondition("PlayerEquippedWeaponTypeLeft", [] {
                const auto actor = RE::PlayerCharacter::GetSingleton();
                if (const auto form = actor->GetEquippedObject(true)) {
                    if (const auto weapon = form->As<RE::TESObjectWEAP>()) {
                        return static_cast<int>(weapon->GetWeaponType());
                    }
                }
                return std::numeric_limits<int>::lowest();
            });
            this->RegisterGlobalCondition("PlayerEquippedWeaponTypeRight", [] {
                if (const auto actor = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto form = actor->GetEquippedObject(false)) {
                        if (const auto weapon = form->As<RE::TESObjectWEAP>()) {
                            return static_cast<int>(weapon->GetWeaponType());
                        }
                    }
                }
                return std::numeric_limits<int>::lowest();
            });
            this->RegisterGlobalCondition("PlayerName", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return player->GetName();
                }
                return "";
            });
            this->RegisterGlobalCondition("PlayerRaceForm", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return player->GetRace();
                }
                return static_cast<RE::TESRace*>(nullptr);
            });
            this->RegisterGlobalCondition("PlayerSex", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto actorBase = player->GetActorBase()) {
                        return static_cast<int>(actorBase->GetSex());
                    }
                }
                return std::numeric_limits<int>::lowest();
            });
            this->RegisterGlobalCondition("PlayerPosX", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return player->GetPositionX();
                }
                return std::numeric_limits<float>::lowest();
            });
            this->RegisterGlobalCondition("PlayerPosY", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return player->GetPositionY();
                }
                return std::numeric_limits<float>::lowest();
            });
            this->RegisterGlobalCondition("PlayerPosZ", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    return player->GetPositionZ();
                }
                return std::numeric_limits<float>::lowest();
            });
            this->RegisterGlobalCondition("PlayerLocationForm", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto loc = player->GetCurrentLocation()) {
                        return loc;
                    }
                }
                return static_cast<RE::BGSLocation*>(nullptr);
            });
            this->RegisterGlobalCondition("PlayerLocationKeywords", [] {
                if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                    if (const auto loc = player->GetCurrentLocation()) {
                        return FormUtil::ToKeywordString(loc);
                    }
                }
                return std::string("");
            });
            this->RegisterGlobalCondition("RandomInt", [] { return Util::RandomInt(1, 99); });
            this->RegisterGlobalCondition("RandomFloat", [] { return Util::RandomFloat(1.0f, 99.0f); });
            this->RegisterGlobalCondition("RandomBool", [] { return Util::RandomInt(1, 99) == 1; });
            this->RegisterGlobalCondition("PlayerWerewolfState", [] {
                if (const auto globalVar = RE::TESForm::LookupByID(0xed06c)->As<RE::TESGlobal>()) {
                    if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                        if (globalVar->value == 1.0f) {
                            if (player->GetRace()->GetLocalFormID() == 0xCDD84) {
                                return 2;
                            }
                            return 1;
                        }
                    }
                }
                return 0;
            });
            this->RegisterGlobalCondition("PlayerVampireState", [] {
                if (const auto globalVar = RE::TESForm::LookupByID(0xed06D)->As<RE::TESGlobal>()) {
                    if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                        if (globalVar->value == 1.0f) {
                            if (player->GetRace()->GetLocalFormID() == 0x283A) {
                                return 2;
                            }
                            return 1;
                        }
                    }
                }
                return 0;
            });
        }

        static ConditionManager* GetSingleton() {
            static ConditionManager instance;
            return &instance;
        }

        ConditionManager(const ConditionManager&) = delete;
        ConditionManager& operator=(const ConditionManager&) = delete;

        /**
         * Registers a condition for the given event (or globally)
         *
         * NOTE: If an event has been played (or failed), all events attached to it will be unregistered automatically. There is no need to do that yourself.
         * @param eventName the name of the event to attach, or GLOBAL to make it globally available
         * @param conditionName the name of the condition
         * @param conditionFunction a function whose return value will be used for evaluation.
         */
        void RegisterCondition(const std::string& eventName, const std::string& conditionName, const std::function<Value()>& conditionFunction) {
            auto& conditionList = conditions[eventName];
            std::ranges::replace_if(conditionList, [&](const Condition& condition) { return condition.name == conditionName; }, Condition{conditionName, conditionFunction});
            if (!std::ranges::any_of(conditionList, [&](const Condition& condition) { return condition.name == conditionName; })) conditionList.emplace_back(Condition{conditionName, conditionFunction});
        }

        /**
         * Convenience-Function, see above for info
         * @param conditionName the name of the condition
         * @param conditionFunction a function whose return value will be used for evaluation.
         */
        void RegisterGlobalCondition(const std::string& conditionName, const std::function<Value()>& conditionFunction) {
            RegisterCondition("GLOBAL", conditionName, conditionFunction);
        }

        /**
         * Unregisters all conditions for the given event.
         *
         * NOTE: You should never use this, unless you absolutely know what you're doing - This will be done automatically after an event played.
         * @param eventName the name of the event
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
            std::function<Value()> function;
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
            Value literal;

            CondExpr(const std::string& v, const std::string& o, const Value& lit) : var(v), op(o), literal(lit) {
            }

            bool eval(const std::string& eventName) const override {
                Value val;
                auto searchConditions = [&](const std::string& key) -> std::optional<Value> {
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
                } else if (auto globalVal = searchConditions("GLOBAL"); globalVal.has_value()) {
                    val = globalVal.value();
                } else {
                    return false;
                }

                if (std::holds_alternative<RE::TESForm*>(val) && (op == "==" || op == "!=")) {
                    const auto lhs = std::get<RE::TESForm*>(val);
                    std::string rhs = "";
                    if (std::holds_alternative<std::string>(literal)) {
                        rhs = std::get<std::string>(literal);
                    } else {
                        return false;
                    }
                    const bool result = FormUtil::CompareForms(lhs, rhs);
                    return op == "==" ? result : !result;
                }
                if (std::holds_alternative<float>(val)) {
                    const float lhs = std::get<float>(val);
                    float rhs = 0.0f;
                    if (std::holds_alternative<float>(literal)) {
                        rhs = std::get<float>(literal);
                    } else if (std::holds_alternative<int>(literal)) {
                        rhs = static_cast<float>(std::get<int>(literal));
                    } else {
                        return false;
                    }
                    Util::LogDebug("{} | {}", lhs, rhs);
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    if (op == "<") return lhs < rhs;
                    if (op == "<=") return lhs <= rhs;
                    if (op == ">") return lhs > rhs;
                    if (op == ">=") return lhs >= rhs;
                } else if (std::holds_alternative<int>(val)) {
                    const int lhs = std::get<int>(val);
                    int rhs = 0;
                    if (std::holds_alternative<int>(literal)) {
                        rhs = std::get<int>(literal);
                    } else if (std::holds_alternative<float>(literal)) {
                        rhs = static_cast<int>(std::get<float>(literal));
                    } else {
                        return false;
                    }
                    Util::LogDebug("{} | {}", lhs, rhs);
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    if (op == "<") return lhs < rhs;
                    if (op == "<=") return lhs <= rhs;
                    if (op == ">") return lhs > rhs;
                    if (op == ">=") return lhs >= rhs;
                    return false;
                } else if (std::holds_alternative<bool>(val)) {
                    const bool lhs = std::get<bool>(val);
                    bool rhs = false;
                    if (std::holds_alternative<bool>(literal)) {
                        rhs = std::get<bool>(literal);
                    } else {
                        return false;
                    }
                    Util::LogDebug("{} | {}", lhs, rhs);
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    return false;
                } else if (std::holds_alternative<std::string>(val)) {
                    const std::string& lhs = std::get<std::string>(val);
                    std::string rhs = "";
                    if (std::holds_alternative<std::string>(literal)) {
                        rhs = std::get<std::string>(literal);
                    } else {
                        return false;
                    }
                    Util::LogDebug("{} | {}", lhs, rhs);
                    if (op == "==") return lhs == rhs;
                    if (op == "!=") return lhs != rhs;
                    if (op == "<") return lhs < rhs;
                    if (op == "<=") return lhs <= rhs;
                    if (op == ">") return lhs > rhs;
                    if (op == ">=") return lhs >= rhs;
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
                    return std::make_unique<CondExpr>(varName, op, Value(num));
                }
                if (current().type == TokenType::Int) {
                    int num = std::stoi(current().value, nullptr, current().value.starts_with("0x") ? 16 : 10);
                    consume();
                    return std::make_unique<CondExpr>(varName, op, Value(num));
                }
                if (current().type == TokenType::Bool) {
                    auto s = current().value;
                    std::ranges::transform(s, s.begin(), [](const unsigned char c) { return std::tolower(c); });
                    consume();
                    return std::make_unique<CondExpr>(varName, op, Value(s == "true"));
                }
                if (current().type == TokenType::String) {
                    std::string s = current().value;
                    consume();
                    return std::make_unique<CondExpr>(varName, op, Value(s));
                }
                return nullptr;
            }
        };

        std::unordered_map<std::string, std::vector<Condition>> conditions;
    };
}