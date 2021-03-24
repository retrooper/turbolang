#pragma once

#include "tokenizer/Tokenizer.h"
#include "function/FunctionCallProcessor.h"
#include "function/Function.h"
#include "utils/Compiler.h"
#include "utils/Type.h"
#include "utils/DataType.h"
#include "data/Statement.h"
#include "utils/SourceCodeReader.h"
#include "math/MathEvaluator.h"
#include <vector>
#include <stack>

namespace turbolang {
    class Parser {
        friend class Compiler;

    public:
        Parser();

        static void parse(std::vector<Token> &tokens);
    private:
        static std::vector<Token>::iterator currentToken;
        static std::vector<Token>::iterator endToken;
        static std::string currentFuncName;

        static bool expectFunctionDefinition();

        static std::optional<std::vector<Statement>> parseFunctionBody();

        static std::optional<Token> expectTokenType(const TokenType &type, const std::string &name);

        static std::optional<Token> expectTokenType(const TokenType &type);

        static std::optional<Token> expectToken();

        static std::optional<DataType> expectTokenVariableType();

        static std::optional<DataType> expectTokenFunctionType();

        static std::optional<bool> expectBooleanValue();

        static std::optional<Statement> parseStatement();

        static void parseFunctionDeclareStatement();

        static void parseImportStatement();

        static void parseClassDefinition();

        static void parseVariableDeclaration(Statement &statement, const Token &typeToken);

        static void parseVariableModification(Statement &statement, const std::optional<Token> &variableName);

        static llvm::Value *parseFunctionCall(Statement &statement, const std::string &functionName);

        static void parseWhileLoop(Statement &statement);

        static void parseReturn(Statement &statement);

        static bool isMathematicalOperator(const std::string& op);

        static llvm::Value* parseMathematicalExpression();
    };
}