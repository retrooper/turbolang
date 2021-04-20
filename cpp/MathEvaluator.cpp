#include "math/MathEvaluator.h"

namespace turbolang {
    std::map<std::string, int> MathEvaluator::operatorPrecedenceMap = {
            //Addition, subtraction
            {"+",  1},
            {"-",  1},
            //Multiplication, division
            {"*",  10},
            {"/",  10},
            //Language operators
            {"==", 20},
            {"!=", 20},
            {"<",  20},
            {">",  20},
            {"<=", 20},
            {">=", 20}
    };

    llvm::Value *
    MathEvaluator::eval(const std::vector<Token> &tokens, Function &currentFunction, const DataType &resultType) {
        std::string s;
        for (const auto &a : tokens) {
            s += a.text + " ";
        }
        llvm::outs() << "Evaluating Expression: " << s << "\n";
        if (s.find('*') == 0) {
            //Dereference
            std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
            s.erase(end_pos, s.end());
            llvm::outs() << s.substr(1) << " we got " << "\n";
            return currentFunction.getDereferencedValue(s.substr(1));
        }


        std::deque<llvm::Value *> numberQueue;
        std::stack<std::string> operatorStack;
        for (const Token &token : tokens) {
            if (token.type == TOKEN_TYPE_INTEGER_LITERAL) {
                numberQueue.push_back(llvm::ConstantInt::get(
                        Type::getLLVMType(resultType),
                        llvm::APInt(32, std::stoi(token.text))));
            } else if (token.type == TOKEN_TYPE_DOUBLE_LITERAL) {
                numberQueue.push_back(llvm::ConstantFP::get(Type::getLLVMType(resultType), std::stod(token.text)));
            } else if (token.type == TOKEN_TYPE_IDENTIFIER) {
                llvm::Value *variableValue;
                if (token.text == "true") {
                    variableValue = llvm::ConstantInt::get(Type::getLLVMType(resultType), llvm::APInt(1, 1));
                } else if (token.text == "false") {
                    variableValue = llvm::ConstantInt::get(Type::getLLVMType(resultType), llvm::APInt(1, 0));
                } else {
                    variableValue = currentFunction.getValue(token.text);
                }
                numberQueue.push_back(variableValue);
            } else if (token.text == "(") {
                operatorStack.push(token.text);
            } else if (isOperator(token)) {
                while (!operatorStack.empty() &&
                       operatorPrecedenceMap[operatorStack.top()] >= operatorPrecedenceMap[token.text]) {
                    llvm::Value *val2 = numberQueue.front();
                    numberQueue.pop_front();

                    llvm::Value *val1 = numberQueue.front();
                    numberQueue.pop_front();

                    std::string operatorType = operatorStack.top();
                    operatorStack.pop();
                    numberQueue.push_back(calculate(val1, val2, operatorType));
                }

                operatorStack.push(token.text);
            } else if (token.text == ")") {
                while (!operatorStack.empty() && operatorStack.top() != "(") {
                    llvm::Value *val2 = numberQueue.front();
                    numberQueue.pop_front();

                    llvm::Value *val1 = numberQueue.front();
                    numberQueue.pop_front();

                    std::string operatorType = operatorStack.top();
                    operatorStack.pop();

                    numberQueue.push_back(calculate(val1, val2, operatorType));
                }
                operatorStack.pop();
            }
        }

        while (!operatorStack.empty()) {
            llvm::Value *val2 = numberQueue.front();
            numberQueue.pop_front();

            llvm::Value *val1 = numberQueue.front();
            numberQueue.pop_front();

            std::string operatorType = operatorStack.top();
            operatorStack.pop();
            numberQueue.push_back(calculate(val1, val2, operatorType));
        }
        return numberQueue.front();
    }

    llvm::Value *MathEvaluator::calculate(llvm::Value *a, llvm::Value *b, const std::string &operatorType) {
        /**
         * MATHEMATICAL OPERATIONS
         */
        if (operatorType == "+") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateAdd(a, b);
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFAdd(a, b);
            }
        } else if (operatorType == "-") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateSub(a, b);
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFSub(a, b);
            }
        } else if (operatorType == "*") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateMul(a, b);
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFMul(a, b);
            }
        } else if (operatorType == "/") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                //TODO check if int is signed or unsigned, default to signed for now
                return LLVMManager::llvmBytecodeBuilder->CreateSDiv(a, b);
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFDiv(a, b);
            }
        }
            /**
             * LANGUAGE OPERATIONS
             */
        else if (operatorType == "==") {
            return LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(a, b);
        } else if (operatorType == "!=") {
            return LLVMManager::llvmBytecodeBuilder->CreateICmpNE(a, b);
        } else if (operatorType == ">") {
            return LLVMManager::llvmBytecodeBuilder->CreateICmpSGT(a, b);
        } else if (operatorType == "<") {
            return LLVMManager::llvmBytecodeBuilder->CreateICmpSLT(a, b);
        } else if (operatorType == "<=") {
            return LLVMManager::llvmBytecodeBuilder->CreateICmpSLE(a, b);
        } else if (operatorType == ">=") {
            return LLVMManager::llvmBytecodeBuilder->CreateICmpSGE(a, b);
        }
        return nullptr;
    }

    bool MathEvaluator::isOperator(const turbolang::Token &token) {
        return token.type == TOKEN_TYPE_OPERATOR &&
               operatorPrecedenceMap.find(token.text) != operatorPrecedenceMap.end();
    }
}