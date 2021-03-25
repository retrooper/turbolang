#include "math/MathEvaluator.h"
namespace turbolang {
    std::map<std::string, int> MathEvaluator::operatorPrecedenceMap = {
            {"+", 1},
            {"-", 1},
            {"*", 10},
            {"/", 10},
    };

    llvm::Value *MathEvaluator::eval(const std::vector<Token> &tokens, Function& currentFunction) {
        std::stack<llvm::Value *> numberStack;
        std::stack<std::string> operatorStack;
        for (const Token &token : tokens) {
            if (token.type == TOKEN_TYPE_INTEGER_LITERAL) {
                numberStack.push(llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_INT), llvm::APInt(32, std::stoi(token.text))));
            }
            else if (token.type == TOKEN_TYPE_DOUBLE_LITERAL) {
                numberStack.push(llvm::ConstantFP::get(Type::getLLVMType(DATA_TYPE_DOUBLE), std::stod(token.text)));
            }
            else if (token.type == TOKEN_TYPE_IDENTIFIER) {
                llvm::Value* variableValue = currentFunction.getValue(token.text);
                numberStack.push(variableValue);
            }
            else if (token.text == "(") {
                operatorStack.push(token.text);
            }
            else if (isOperator(token)) {
                while (!operatorStack.empty() &&
                       operatorPrecedenceMap[operatorStack.top()] >= operatorPrecedenceMap[token.text]) {
                    llvm::Value *val2 = numberStack.top();
                    numberStack.pop();

                    llvm::Value *val1 = numberStack.top();
                    numberStack.pop();

                    std::string operatorType = operatorStack.top();
                    operatorStack.pop();

                    numberStack.push(calculate(val1, val2, operatorType));
                }

                operatorStack.push(token.text);
            }
            else if (token.text == ")") {
                while(!operatorStack.empty() && operatorStack.top() != "(")
                {
                    llvm::Value* val2 = numberStack.top();
                    numberStack.pop();

                    llvm::Value* val1 = numberStack.top();
                    numberStack.pop();

                    std::string operatorType = operatorStack.top();
                    operatorStack.pop();

                    numberStack.push(calculate(val1, val2, operatorType));
                }
                operatorStack.pop();
            }
        }

        while (!operatorStack.empty()) {
            llvm::Value *val2 = numberStack.top();
            numberStack.pop();

            llvm::Value *val1 = numberStack.top();
            numberStack.pop();

            std::string operatorType = operatorStack.top();
            operatorStack.pop();

            numberStack.push(calculate(val1, val2, operatorType));
        }
        return numberStack.top();
    }

    llvm::Value *MathEvaluator::calculate(llvm::Value *a, llvm::Value *b, const std::string &operatorType) {
        if (operatorType == "+") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateAdd(a, b);
            }
            else {
                return LLVMManager::llvmBytecodeBuilder->CreateFAdd(a, b);
            }
        }
        else if (operatorType == "-") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateSub(a, b);
            }
            else {
                return LLVMManager::llvmBytecodeBuilder->CreateFSub(a, b);
            }
        }
        else if (operatorType == "*") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateMul(a, b);
            }
            else {
                return LLVMManager::llvmBytecodeBuilder->CreateFMul(a, b);
            }
        }
        else if (operatorType == "/") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                //TODO check if int is signed or unsigned, default to signed for now
                return LLVMManager::llvmBytecodeBuilder->CreateSDiv(a, b);
            }
            else {
                return LLVMManager::llvmBytecodeBuilder->CreateFDiv(a, b);
            }
        }
        return nullptr;
    }

    bool MathEvaluator::isOperator(const turbolang::Token &token) {
        return token.type == TOKEN_TYPE_OPERATOR &&
               operatorPrecedenceMap.find(token.text) != operatorPrecedenceMap.end();
    }
}