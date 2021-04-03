#pragma once

#include "token/Tokenizer.h"
#include "function/FunctionCallProcessor.h"
#include "function/Function.h"
#include "class/Class.h"
#include "utils/Compiler.h"
#include "utils/Type.h"
#include "utils/DataType.h"
#include "utils/SourceCodeReader.h"
#include "math/MathEvaluator.h"
#include <vector>
#include <stack>
#include <functional>
namespace turbolang {
    class Parser {
        friend class Compiler;
    public:
        static void parse(std::vector<Token> &tokens);
    private:
        static std::vector<Token>::iterator currentToken;
        static std::vector<Token>::iterator endToken;
        static std::string currentFuncName;

        static bool expectFunctionDefinition();

        static void parseFunctionBody();

        static std::optional<Token> expectTokenType(const TokenType &type, const std::string &name);

        static std::optional<Token> expectTokenType(const TokenType &type);

        static std::optional<Token> expectToken();

        static std::optional<DataType> expectTokenDataType();

        static llvm::Value* expectExpression(const Token* token = nullptr, const std::string& endAtStr = ";",
                                             const std::function<void(std::vector<Token>&)>& extraProcessing = [](std::vector<Token>& tokens){});

        static void parseStatement();

        static void parseFunctionDeclareStatement();

        static void parseImportStatement();

        static void parseClassDefinition();

        static void parseVariableDeclaration(const std::optional<DataType>& varType, const std::string& className= "");

        static void parseVariableModification(const std::optional<Token> &variableName);

        static llvm::Value *parseFunctionCall(const std::string &functionName);

        static void parseWhileLoop();

        static void parseReturnStatement();

        static void parseIfStatement();

        static void parseElifStatement(llvm::BasicBlock*& endBlock);

        static void parseElseStatement(llvm::BasicBlock*& endBlock);
    };
}