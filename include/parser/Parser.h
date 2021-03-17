#pragma once

#include "tokenizer/tokenizer.h"
#include "function/FunctionCallProcessor.h"
#include "function/Function.h"
#include "utils/Compiler.h"
#include "utils/Type.h"
#include "data/Statement.h"
#include <vector>

namespace turbolang {
    class Parser {
        friend class Compiler;

    public:
        Parser();

        void parse(std::vector<token> &tokens);

    private:

        std::vector<token>::iterator currentToken;
        std::vector<token>::iterator endToken;
        std::string currentFuncName;

        bool expect_function_definition();

        std::optional<std::vector<Statement>> parse_function_body();

        std::optional<token> expect_token_type(const tokentype &type, const std::string &name);

        std::optional<token> expect_token_type(const tokentype &type);

        std::optional<token> expect_token();

        std::optional<VariableType> expect_token_variable_type();

        std::optional<FunctionType> expect_token_function_type();

        std::optional<bool> expect_boolean_value();

        std::optional<Statement> parseStatement();

        void parseVariableDeclaration(Statement &statement, const token &typeToken);

        void parseVariableModification(Statement &statement, const std::optional<token> &variableName);

        llvm::Value *parseFunctionCall(Statement &statement, const std::string &functionName);

        void parseWhileLoop(Statement &statement);

        void parseReturn(Statement &statement);
    };
}