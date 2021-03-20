#pragma once

#include "tokenizer/Tokenizer.h"
#include "function/FunctionCallProcessor.h"
#include "function/Function.h"
#include "utils/Compiler.h"
#include "utils/Type.h"
#include "data/Statement.h"
#include "utils/SourceCodeReader.h"
#include <vector>
#include <list>
#include <stack>

namespace turbolang {
    class Parser {
        friend class Compiler;

    public:
        Parser();

        static void parse(std::vector<Token> &tokens);
        static void load();
        static void unload();
    private:
        static std::vector<Token>::iterator currentToken;
        static std::vector<Token>::iterator endToken;
        static std::string currentFuncName;

        static bool expect_function_definition();

        static std::optional<std::vector<Statement>> parse_function_body();

        static std::optional<Token> expect_token_type(const TokenType &type, const std::string &name);

        static std::optional<Token> expect_token_type(const TokenType &type);

        static std::optional<Token> expect_token();

        static std::optional<VariableType> expect_token_variable_type();

        static std::optional<FunctionType> expect_token_function_type();

        static std::optional<bool> expect_boolean_value();

        static std::optional<Statement> parseStatement();

        static void parseImportStatement();

        static void parseVariableDeclaration(Statement &statement, const Token &typeToken);

        static void parseVariableModification(Statement &statement, const std::optional<Token> &variableName);

        static llvm::Value *parseFunctionCall(Statement &statement, const std::string &functionName);

        static void parseWhileLoop(Statement &statement);

        static void parseReturn(Statement &statement);

        static bool isMathematicalOperator(const std::string& op);

        static llvm::Value* parseMathematicalExpression();
    };
}