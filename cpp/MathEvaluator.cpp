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

    std::deque<Token>
    MathEvaluator::shuntingYardAlgorithm(const std::vector<Token> &tokens) {
        std::deque<Token> queue;
        std::vector<Token> stack;
        for (const Token &token : tokens) {
            if (token.type == TOKEN_TYPE_INTEGER_LITERAL
                || token.type == TOKEN_TYPE_DOUBLE_LITERAL
                || token.type == TOKEN_TYPE_IDENTIFIER) {
                queue.push_back(token);
            } else if (token.text == "(") {
                stack.push_back(token);
            } else if (token.text == ")") {
                bool match = false;
                while (!stack.empty() && stack.back().text != "(") {
                    queue.push_back(stack.back());
                    stack.pop_back();
                    match = true;
                }
                stack.pop_back();
                if (!match && stack.empty()) {
                    llvm::outs() << "Missing right parenthesis in math expression!\n";
                    std::exit(-1);
                }
            } else if (isOperator(token)) {
                const auto o1 = token;
                while (!stack.empty()) {
                    const auto o2 = stack.back();
                    if (operatorPrecedenceMap[o1.text] <= operatorPrecedenceMap[o2.text]) {
                        stack.pop_back();
                        queue.push_back(o2);
                        continue;
                    }
                    break;
                }
                stack.push_back(o1);
            }
        }
        return queue;
    }

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
        std::deque<Token> queue = shuntingYardAlgorithm(tokens);
        llvm::outs() << "size: " << queue.size() << "\n";
        std::vector<llvm::Value*> stack;
        while(!queue.empty()) {
            const auto token = queue.front();
            queue.pop_front();
            switch(token.type) {
                case TOKEN_TYPE_INTEGER_LITERAL:
                    stack.push_back(llvm::ConstantInt::get(
                            Type::getLLVMType(resultType),
                            llvm::APInt(32, std::stoi(token.text))));//TODO change num bits and support other int types
                    break;
                case TOKEN_TYPE_DOUBLE_LITERAL:
                    stack.push_back(llvm::ConstantFP::get(Type::getLLVMType(resultType), std::stod(token.text)));
                    break;
                case TOKEN_TYPE_IDENTIFIER: {
                    llvm::Value *variableValue;
                    if (token.text == "true") {
                        variableValue = llvm::ConstantInt::get(Type::getLLVMType(resultType), llvm::APInt(1, 1));
                    } else if (token.text == "false") {
                        variableValue = llvm::ConstantInt::get(Type::getLLVMType(resultType), llvm::APInt(1, 0));
                    } else {
                        variableValue = currentFunction.getValue(token.text);
                    }
                    stack.push_back(variableValue);
                    break;
                }
                default:
                    if (isOperator(token)) {
                        const auto rhs = stack.back();
                        stack.pop_back();
                        const auto lhs = stack.back();
                        stack.pop_back();
                        stack.push_back(calculate(lhs, rhs, token.text));
                    }
                    else {
                        //TODO log error
                    }
                    break;
            }
        }

        return stack.back();
    }

    llvm::Value *MathEvaluator::calculate(llvm::Value *a, llvm::Value *b, const std::string &operatorType) {
        llvm::outs() << "operator: " << operatorType << "\n";
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