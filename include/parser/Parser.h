#pragma once

#include "tokenizer/Tokenizer.h"
#include "function/FunctionCallProcessor.h"
#include "function/Function.h"
#include "utils/Compiler.h"
#include "utils/Type.h"
#include "data/Statement.h"
#include <vector>
#include <list>
#include <stack>

namespace turbolang {
    class Parser {
        friend class Compiler;

    public:
        Parser();

        void parse(std::vector<Token> &tokens);

    private:

        std::vector<Token>::iterator currentToken;
        std::vector<Token>::iterator endToken;
        std::string currentFuncName;

        bool expect_function_definition();

        std::optional<std::vector<Statement>> parse_function_body();

        std::optional<Token> expect_token_type(const TokenType &type, const std::string &name);

        std::optional<Token> expect_token_type(const TokenType &type);

        std::optional<Token> expect_token();

        std::optional<VariableType> expect_token_variable_type();

        std::optional<FunctionType> expect_token_function_type();

        std::optional<bool> expect_boolean_value();

        std::optional<Statement> parseStatement();

        void parseVariableDeclaration(Statement &statement, const Token &typeToken);

        void parseVariableModification(Statement &statement, const std::optional<Token> &variableName);

        llvm::Value *parseFunctionCall(Statement &statement, const std::string &functionName);

        void parseWhileLoop(Statement &statement);

        void parseReturn(Statement &statement);

        bool isMathematicalOperator(const std::string& op);

        llvm::Value* parseMathematicalExpression();
    };
}