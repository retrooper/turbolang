#pragma once
#include "utils/llvm/LLVMManager.h"
#include "utils/Type.h"
#include "tokenizer/Tokenizer.h"
#include <stack>
#include <vector>
namespace turbolang {
    class MathEvaluator {
    public:
        static llvm::Value* eval(const std::vector<Token>& tokens);
    private:
        static std::map<std::string, int> operatorPrecedenceMap;
        static llvm::Value* calculate(llvm::Value* a, llvm::Value* b, const std::string& operatorType);
        static bool isOperator(const Token& token);
    };
}