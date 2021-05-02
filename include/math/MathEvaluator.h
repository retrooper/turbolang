#pragma once
#include "utils/llvm/LLVMManager.h"
#include "utils/Type.h"
#include "function/Function.h"
#include "token/Token.h"
#include <stack>
#include <vector>
#include "parser/Parser.h"
namespace turbolang {
    class MathEvaluator {
        friend class Tokenizer;
    public:
        static llvm::Value* eval(std::vector<Token>& tokens, Function& currentFunction, DataType resultType = DATA_TYPE_UNKNOWN, const std::string& className = "");
    private:
        static std::map<std::string, int> operatorPrecedenceMap;
        static std::deque<Token> shuntingYardAlgorithm(std::vector<Token>& tokens);
        static llvm::Value* calculate(llvm::Value* a, llvm::Value* b, const std::string& operatorType);
        static bool isOperator(const Token& token);
    };
}