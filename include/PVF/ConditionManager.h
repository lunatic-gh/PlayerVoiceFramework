#pragma once

#include "API.h"

namespace PVF {
    class ConditionManager {
    public:
        ConditionManager(const ConditionManager&) = delete;

        ConditionManager& operator=(const ConditionManager&) = delete;

        static ConditionManager* GetSingleton();

        void RegisterInternalConditions();

        void RegisterCondition(const std::string& eventName, const std::string& conditionName, const std::function<DataValue()>& valueFunction);

        void UnregisterConditions(const std::string& eventName);

        void RegisterGlobalCondition(const std::string& conditionName, const std::function<DataValue()>& valueFunction);

        [[nodiscard]] bool EvaluateExpression(const std::string& eventName, const std::string& expression) const;

    private:
        ConditionManager();

        struct Condition {
            std::string name;
            std::function<DataValue()> valueFunction;
        };

        enum TokenType {
            kIdentifier,
            kValueString,
            kValueInt,
            kValueFloat,
            kValueBool,
            kOperator,
            kLParen,
            kRParen,
            kEnd
        };

        struct Token {
            TokenType type;
            std::string value;
        };

        class Expr {
        public:
            virtual ~Expr();

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

            LogicalExpr(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r, Op o);

            [[nodiscard]] bool eval(const std::string& eventName) const override;
        };

        class CondExpr final : public Expr {
        public:
            std::string var;
            std::string op;
            DataValue literal;

            CondExpr(std::string v, std::string o, DataValue lit);

            [[nodiscard]] bool eval(const std::string& eventName) const override;
        };

        class Parser {
        public:
            explicit Parser(const std::vector<Token>& tokens);

            std::unique_ptr<Expr> parseExpression();

        private:
            const std::vector<Token>& tokens;
            size_t pos;

            [[nodiscard]] const Token& current() const;

            void consume();

            bool match(TokenType type, const std::string& val = "");

            std::unique_ptr<Expr> parseOr();

            std::unique_ptr<Expr> parseAnd();

            std::unique_ptr<Expr> parseFactor();

            std::unique_ptr<Expr> parseCondition();
        };

        std::unordered_map<std::string, std::vector<Condition> > conditions;
    };
}
