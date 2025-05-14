#include "../include/PVF/ConditionManager.h"

#include "../../include/PVF/Util.h"

namespace PVF {
    ConditionManager* ConditionManager::GetSingleton() {
        static ConditionManager instance;
        return &instance;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void ConditionManager::RegisterInternalConditions() {
        this->RegisterGlobalCondition("PlayerHealthPercentage", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                                              const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
                                              if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth); f2 != 0.0f) {
                                                  const float f = (f1 / f2) * 100.0f;
                                                  return DataValue(f);
                                              }
                                          }
                                          return DataValue(0.0f);
                                      }));

        this->RegisterGlobalCondition("PlayerStaminaPercentage", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                                              const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina);
                                              if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kStamina); f2 != 0.0f) {
                                                  const float f = (f1 / f2) * 100.0f;
                                                  return DataValue(f);
                                              }
                                          }
                                          return DataValue(0.0f);
                                      }));

        this->RegisterGlobalCondition("PlayerMagickaPercentage", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton(); player->AsActorValueOwner() != nullptr) {
                                              const float f1 = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
                                              if (const float f2 = player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kMagicka); f2 != 0.0f) {
                                                  const float f = (f1 / f2) * 100.0f;
                                                  return DataValue(f);
                                              }
                                          }
                                          return DataValue(0.0f);
                                      }));

        this->RegisterGlobalCondition("PlayerEquippedWeaponTypeLeft", CreateConditionFunction([] {
                                          const auto actor = RE::PlayerCharacter::GetSingleton();
                                          if (const auto form = actor->GetEquippedObject(true)) {
                                              if (const auto weapon = form->As<RE::TESObjectWEAP>()) { return DataValue(static_cast<int>(weapon->GetWeaponType())); }
                                          }
                                          return DataValue(std::numeric_limits<int>::lowest());
                                      }));

        this->RegisterGlobalCondition("PlayerEquippedWeaponTypeRight", CreateConditionFunction([] {
                                          if (const auto actor = RE::PlayerCharacter::GetSingleton()) {
                                              if (const auto form = actor->GetEquippedObject(false)) {
                                                  if (const auto weapon = form->As<RE::TESObjectWEAP>()) { return DataValue(static_cast<int>(weapon->GetWeaponType())); }
                                              }
                                          }
                                          return DataValue(std::numeric_limits<int>::lowest());
                                      }));

        this->RegisterGlobalCondition("PlayerName", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) { return DataValue(player->GetName()); }
                                          return DataValue("");
                                      }));

        this->RegisterGlobalCondition("PlayerRaceForm", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                                              // Assuming GetRace() returns a pointer convertible to RE::TESForm*
                                              return DataValue(static_cast<RE::TESForm*>(player->GetRace()));
                                          }
                                          return DataValue(static_cast<RE::TESForm*>(nullptr));
                                      }));

        this->RegisterGlobalCondition("PlayerSex", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                                              if (const auto actorBase = player->GetActorBase()) { return DataValue(static_cast<int>(actorBase->GetSex())); }
                                          }
                                          return DataValue(std::numeric_limits<int>::lowest());
                                      }));

        this->RegisterGlobalCondition("PlayerPosX", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) { return DataValue(player->GetPositionX()); }
                                          return DataValue(std::numeric_limits<float>::lowest());
                                      }));

        this->RegisterGlobalCondition("PlayerPosY", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) { return DataValue(player->GetPositionY()); }
                                          return DataValue(std::numeric_limits<float>::lowest());
                                      }));

        this->RegisterGlobalCondition("PlayerPosZ", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) { return DataValue(player->GetPositionZ()); }
                                          return DataValue(std::numeric_limits<float>::lowest());
                                      }));

        this->RegisterGlobalCondition("PlayerLocationForm", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                                              if (const auto loc = player->GetCurrentLocation()) { return DataValue(static_cast<RE::TESForm*>(loc)); }
                                          }
                                          return DataValue(static_cast<RE::TESForm*>(nullptr));
                                      }));

        this->RegisterGlobalCondition("PlayerLocationKeywords", CreateConditionFunction([] {
                                          if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                                              if (const auto loc = player->GetCurrentLocation()) {
                                                  // Assuming FormUtil::ToKeywordString returns a std::string.
                                                  return DataValue(FormUtil::ToKeywordString(loc).c_str());
                                              }
                                          }
                                          return DataValue("");
                                      }));

        this->RegisterGlobalCondition("RandomInt", CreateConditionFunction([] { return DataValue(Util::RandomInt(1, 99)); }));
        this->RegisterGlobalCondition("RandomFloat", CreateConditionFunction([] { return DataValue(Util::RandomFloat(1.0f, 99.0f)); }));
        this->RegisterGlobalCondition("RandomBool", CreateConditionFunction([] { return DataValue(Util::RandomInt(1, 99) == 1); }));

        this->RegisterGlobalCondition("PlayerWerewolfState", CreateConditionFunction([] {
                                          if (const auto globalVar = RE::TESForm::LookupByID(0xed06c)->As<RE::TESGlobal>()) {
                                              if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                                                  if (globalVar->value == 1.0f) {
                                                      if (player->GetRace()->GetLocalFormID() == 0xCDD84) { return DataValue(2); }
                                                      return DataValue(1);
                                                  }
                                              }
                                          }
                                          return DataValue(0);
                                      }));

        this->RegisterGlobalCondition("PlayerVampireState", CreateConditionFunction([] {
                                          if (const auto globalVar = RE::TESForm::LookupByID(0xed06D)->As<RE::TESGlobal>()) {
                                              if (const auto player = RE::PlayerCharacter::GetSingleton()) {
                                                  if (globalVar->value == 1.0f) {
                                                      if (player->GetRace()->GetLocalFormID() == 0x283A) { return DataValue(2); }
                                                      return DataValue(1);
                                                  }
                                              }
                                          }
                                          return DataValue(0);
                                      }));
    }

    void ConditionManager::RegisterCondition(const std::string& eventName, const std::string& conditionName, const ConditionFunction& valueFunction) {
        auto& conditionList = conditions[eventName];
        if (const auto it = std::ranges::find_if(conditionList, [&](const Condition& condition) { return condition.name == conditionName; }); it != conditionList.end()) {
            if (it->valueFunction.cleanup) { it->valueFunction.cleanup(it->valueFunction.context); }
            *it = Condition{conditionName, valueFunction};
        } else {
            conditionList.emplace_back(Condition{conditionName, valueFunction});
        }
    }

    void ConditionManager::UnregisterConditions(const std::string& eventName) {
        if (eventName == "GLOBAL") { return; }
        conditions.erase(eventName);
    }

    void ConditionManager::RegisterGlobalCondition(const std::string& conditionName, const ConditionFunction& valueFunction) {
        RegisterCondition("GLOBAL", conditionName, valueFunction);
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool ConditionManager::EvaluateExpression(const std::string& eventName, const std::string& expression) const { // NOLINT(*-convert-member-functions-to-static)
        if (expression.empty() || expression.find_first_not_of(' ') == std::string::npos) { return true; }
        std::vector<Token> tokens;
        int i = 0;
        while (i < expression.size()) {
            if (std::isspace(expression[i])) {
                i++;
                continue;
            }
            if (i + 1 < expression.size() && expression[i] == '0' && (expression[i + 1] == 'x' || expression[i + 1] == 'X')) {
                int j = i + 2;
                while (j < expression.size() && std::isxdigit(expression[j])) {
                    j++;
                }
                tokens.push_back({kValueInt, expression.substr(i, j - i)});
                i = j;
                continue;
            }
            if (std::isdigit(expression[i]) || (expression[i] == '.' && i + 1 < expression.size() && std::isdigit(expression[i + 1]))) {
                int j = i;
                bool dotEncountered = false;
                while (j < expression.size() && (std::isdigit(expression[j]) || expression[j] == '.')) {
                    if (expression[j] == '.') {
                        if (dotEncountered) break;
                        dotEncountered = true;
                    }
                    j++;
                }
                tokens.push_back({dotEncountered ? kValueFloat : kValueInt, expression.substr(i, j - i)});
                i = j;
                continue;
            }
            if (std::isalpha(expression[i])) {
                int j = i;
                while (j < expression.size() && std::isalnum(expression[j])) {
                    j++;
                }
                if (std::string tokenStr = expression.substr(i, j - i); tokenStr == "true" || tokenStr == "false") {
                    tokens.push_back({kValueBool, tokenStr});
                } else {
                    tokens.push_back({kIdentifier, tokenStr});
                }
                i = j;
                continue;
            }
            if (expression[i] == '\'' || expression[i] == '"') {
                const char quote = expression[i];
                i++;
                std::string literal;
                while (i < expression.size()) {
                    if (expression[i] == quote) {
                        if (i + 1 < expression.size() && expression[i + 1] == quote) {
                            literal.push_back(quote);
                            i += 2;
                        } else {
                            i++;
                            break;
                        }
                    } else if (expression[i] == '\\') {
                        if (i + 1 < expression.size()) {
                            literal.push_back(expression[i + 1]);
                            i += 2;
                        } else {
                            literal.push_back(expression[i]);
                            i++;
                        }
                    } else {
                        literal.push_back(expression[i]);
                        i++;
                    }
                }
                tokens.push_back({kValueString, literal});
                continue;
            }
            if (expression[i] == '(') {
                tokens.push_back({kLParen, "("});
                ++i;
                continue;
            }
            if (expression[i] == ')') {
                tokens.push_back({kRParen, ")"});
                ++i;
                continue;
            }
            if (i + 1 < expression.size()) {
                if (std::string two = expression.substr(i, 2); two == "&&" || two == "||" || two == "==" || two == "!=" || two == "<=" || two == ">=" || two == "**" || two == "!*") {
                    tokens.push_back({kOperator, two});
                    i += 2;
                    continue;
                }
            }
            if (expression[i] == '<' || expression[i] == '>') {
                tokens.push_back({kOperator, std::string(1, expression[i])});
                ++i;
                continue;
            }
            ++i;
        }
        tokens.push_back({kEnd, ""});
        Parser parser(tokens);
        const auto parsedExpr = parser.parseExpression();
        return parsedExpr && parsedExpr->eval(eventName);
    }

    ConditionManager::ConditionManager() = default;

    ConditionManager::Expr::~Expr() = default;

    ConditionManager::LogicalExpr::LogicalExpr(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r, const Op o) : left(std::move(l)), right(std::move(r)), op(o) {
    }

    bool ConditionManager::LogicalExpr::eval(const std::string& eventName) const {
        if (op == Op::And) { return left->eval(eventName) && right->eval(eventName); }
        return left->eval(eventName) || right->eval(eventName);
    }

    ConditionManager::CondExpr::CondExpr(std::string v, std::string o, DataValue lit) : var(std::move(v)), op(std::move(o)), literal(std::move(lit)) {
    }

    bool ConditionManager::CondExpr::eval(const std::string& eventName) const {
        DataValue val;
        auto searchConditions = [&](const std::string& key) -> std::optional<DataValue> {
            if (GetSingleton()->conditions.contains(key)) {
                auto eventConditions = GetSingleton()->conditions[key];
                if (const auto it = std::ranges::find_if(eventConditions, [&](const auto& cond) { return cond.name == var; }); it != eventConditions.end()) {
                    const auto fnct = it->valueFunction;
                    return fnct.callback(fnct.context);
                }
            }
            return std::nullopt;
        };

        bool b = false;
        if (const auto globalVal = searchConditions("GLOBAL"); globalVal.has_value()) {
            b = true;
            val = globalVal.value();
        }

        if (const auto eventVal = searchConditions(eventName); eventVal.has_value()) {
            b = true;
            val = eventVal.value();
        }

        if (!b) return false;

        if (val.GetType() == DataValue::Type::kForm && literal.GetType() == DataValue::Type::kString && (op == "==" || op == "!=")) {
            const auto lhs = val.AsForm();
            std::string rhs = literal.AsString();
            const bool result = FormUtil::CompareForms(lhs, rhs);
            return op == "==" ? result : !result;
        }
        if (val.GetType() == DataValue::Type::kFloat) {
            float lhs = val.AsFloat();
            float rhs = 0.0f;
            if (literal.GetType() == DataValue::Type::kFloat) {
                rhs = literal.AsFloat();
            } else if (literal.GetType() == DataValue::Type::kInt) {
                rhs = static_cast<float>(literal.AsInt());
            } else {
                return false;
            }
            if (op == "==") return lhs == rhs;
            if (op == "!=") return lhs != rhs;
            if (op == "<") return lhs < rhs;
            if (op == "<=") return lhs <= rhs;
            if (op == ">") return lhs > rhs;
            if (op == ">=") return lhs >= rhs;
        } else if (val.GetType() == DataValue::Type::kInt) {
            int lhs = val.AsInt();
            int rhs;
            if (literal.GetType() == DataValue::Type::kInt) {
                rhs = literal.AsInt();
            } else if (literal.GetType() == DataValue::Type::kFloat) {
                rhs = static_cast<int>(literal.AsFloat());
            } else {
                return false;
            }
            if (op == "==") return lhs == rhs;
            if (op == "!=") return lhs != rhs;
            if (op == "<") return lhs < rhs;
            if (op == "<=") return lhs <= rhs;
            if (op == ">") return lhs > rhs;
            if (op == ">=") return lhs >= rhs;
        } else if (val.GetType() == DataValue::Type::kBool && literal.GetType() == DataValue::Type::kBool) {
            bool lhs = val.AsBool();
            bool rhs = literal.AsBool();
            if (op == "==") return lhs == rhs;
            if (op == "!=") return lhs != rhs;
        } else if (val.GetType() == DataValue::Type::kString && literal.GetType() == DataValue::Type::kString) {
            auto lhs = val.AsString();
            auto rhs = val.AsString();
            if (op == "==") return lhs == rhs;
            if (op == "!=") return lhs != rhs;
            if (op == "**") return lhs.contains(rhs);
            if (op == "!*") return !lhs.contains(rhs);
            return false;
        }
        return false;
    }

    ConditionManager::Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {
    }

    std::unique_ptr<ConditionManager::Expr> ConditionManager::Parser::parseExpression() { // NOLINT(*-no-recursion)
        return parseOr();
    }

    const ConditionManager::Token& ConditionManager::Parser::current() const {
        return tokens[pos];
    }

    void ConditionManager::Parser::consume() {
        pos++;
    }

    bool ConditionManager::Parser::match(const TokenType type, const std::string& val) {
        if (current().type == type && (val.empty() || current().value == val)) {
            consume();
            return true;
        }
        return false;
    }

    std::unique_ptr<ConditionManager::Expr> ConditionManager::Parser::parseOr() { // NOLINT(*-no-recursion)
        auto left = parseAnd();
        while (current().type == kOperator && current().value == "||") {
            consume();
            auto right = parseAnd();
            left = std::make_unique<LogicalExpr>(std::move(left), std::move(right), LogicalExpr::Op::Or);
        }
        return left;
    }

    std::unique_ptr<ConditionManager::Expr> ConditionManager::Parser::parseAnd() { // NOLINT(*-no-recursion)
        auto left = parseFactor();
        while (current().type == kOperator && current().value == "&&") {
            consume();
            auto right = parseFactor();
            left = std::make_unique<LogicalExpr>(std::move(left), std::move(right), LogicalExpr::Op::And);
        }
        return left;
    }

    std::unique_ptr<ConditionManager::Expr> ConditionManager::Parser::parseFactor() { // NOLINT(*-no-recursion)
        if (match(kLParen)) {
            auto expr = parseExpression();
            match(kRParen);
            return expr;
        }
        return parseCondition();
    }

    std::unique_ptr<ConditionManager::Expr> ConditionManager::Parser::parseCondition() {
        if (current().type != kIdentifier) { return nullptr; }
        std::string varName = current().value;
        consume();

        if (current().type != kOperator) { return nullptr; }
        std::string op = current().value;
        consume();

        if (current().type == kValueFloat) {
            const float num = std::stof(current().value);
            consume();
            return std::make_unique<CondExpr>(varName, op, DataValue(num));
        }
        if (current().type == kValueInt) {
            const int num = std::stoi(current().value, nullptr, current().value.starts_with("0x") ? 16 : 10);
            consume();
            return std::make_unique<CondExpr>(varName, op, DataValue(num));
        }
        if (current().type == kValueBool) {
            auto s = current().value;
            std::ranges::transform(s, s.begin(), [](const unsigned char c) { return std::tolower(c); });
            consume();
            return std::make_unique<CondExpr>(varName, op, DataValue(s == "true"));
        }
        if (current().type == kValueString) {
            const auto s = current().value.c_str();
            consume();
            return std::make_unique<CondExpr>(varName, op, DataValue(s));
        }
        return nullptr;
    }
}
