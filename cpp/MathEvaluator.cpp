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
    MathEvaluator::shuntingYardAlgorithm(std::vector<Token> &tokens) {
        std::deque<Token> queue;
        std::vector<Token> stack;
        for (const Token &token : tokens) {
            if (token.type == TOKEN_TYPE_INTEGER_LITERAL
                || token.type == TOKEN_TYPE_DOUBLE_LITERAL
                || token.type == TOKEN_TYPE_IDENTIFIER
                || token.type == TOKEN_TYPE_STRING_LITERAL) {
                queue.push_back(token);
            } else if (token.text == "(") {
                stack.push_back(token);
            } else if (token.text == ")") {
                while (!stack.empty() && stack.back().text != "(") {
                    queue.push_back(stack.back());
                    stack.pop_back();
                }
                stack.pop_back();
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
    MathEvaluator::eval(std::vector<Token> &tokens, Function &currentFunction, DataType resultType, const std::string& className) {
        std::string s;
        for (const auto &a : tokens) {
            s += a.text + " ";
        }
        llvm::outs() << "Evaluating Expression: " << s << "\n";
        if (s.find('*') == 0) {
            //Dereference
            std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
            s.erase(end_pos, s.end());
            return currentFunction.getDereferencedValue(s.substr(1));
        }
        else if (s.find('&') == 0) {
            std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
            s.erase(end_pos, s.end());
            return currentFunction.getAllocaInst(s.substr(1));
        }
        Token firstParenthesis;
        firstParenthesis.text = "(";
        firstParenthesis.type = TOKEN_TYPE_OPERATOR;
        firstParenthesis.lineNumber = tokens[0].lineNumber;
        Token secondParenthesis;
        secondParenthesis.text = ")";
        secondParenthesis.type = TOKEN_TYPE_OPERATOR;
        secondParenthesis.lineNumber = tokens[0].lineNumber;
        tokens.insert(tokens.begin(), 1, firstParenthesis);
        tokens.push_back(secondParenthesis);
        std::deque<Token> queue = shuntingYardAlgorithm(tokens);
        std::vector<llvm::Value*> stack;
        while(!queue.empty()) {
            const auto token = queue.front();
            queue.pop_front();
            switch(token.type) {
                case TOKEN_TYPE_INTEGER_LITERAL: {
                    resultType = resultType == DATA_TYPE_UNKNOWN ? DATA_TYPE_INT : resultType;
                    stack.push_back(llvm::ConstantInt::get(
                            Type::getLLVMType(resultType),
                            llvm::APInt(Type::getBitCount(resultType),
                                        std::stoi(token.text))));//TODO change num bits and support other int types
                    break;
                }
                case TOKEN_TYPE_DOUBLE_LITERAL:
                    resultType = resultType == DATA_TYPE_UNKNOWN ? DATA_TYPE_DOUBLE : resultType;
                    stack.push_back(llvm::ConstantFP::get(Type::getLLVMType(resultType), std::stod(token.text)));
                    break;
                case TOKEN_TYPE_STRING_LITERAL:
                    stack.push_back(LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(token.text));
                    break;
                case TOKEN_TYPE_IDENTIFIER: {
                    llvm::Value *variableValue;
                    if (token.text == "true") {
                        variableValue = llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1));
                    } else if (token.text == "false") {
                        variableValue = llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 0));
                    }
                    else if (token.text == "nullptr") {
                        variableValue = llvm::ConstantPointerNull::get(llvm::PointerType::get(Type::getLLVMType(resultType == DATA_TYPE_UNKNOWN ? DATA_TYPE_VOID : resultType, className), 0));
                    }
                    else {
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