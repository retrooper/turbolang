#pragma once
#include "utils/llvm/LLVMManager.h"
#include "utils/Type.h"
#include "function/Function.h"
#include "token/Token.h"
#include <stack>
#include <vector>
namespace turbolang {
    class MathEvaluator {
        friend class Tokenizer;
    public:
        static llvm::Value* eval(std::vector<Token>& tokens, Function& currentFunction, const DataType& resultType);
    private:
        static std::map<std::string, int> operatorPrecedenceMap;
        static std::deque<Token> shuntingYardAlgorithm(std::vector<Token>& tokens);
        static llvm::Value* calculate(llvm::Value* a, llvm::Value* b, const std::string& operatorType);
        static bool isOperator(const Token& token);
    };
}