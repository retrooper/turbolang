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
    MathEvaluator::eval(std::vector<Token> &tokens, Function &currentFunction, DataType resultType,
                        const std::string &className) {
        std::string s;
        for (const auto &a : tokens) {
            s += a.text + " ";
        }
        LOG_INFO("Evaluating Expression: " << s);
        if (s.find('*') == 0) {
            //Dereference
            std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
            s.erase(end_pos, s.end());
            return currentFunction.getDereferencedValue(s.substr(1));
        } else if (s.find('&') == 0) {
            if (tokens.size() > 3 && tokens[1].text == "[" && tokens[tokens.size() - 1].text == "]") {
                std::string arrayName = tokens[0].text.substr(1);
                //TODO allow expressions in the []
                Token innerToken = tokens[2];
                llvm::AllocaInst *arrayAllocaInst = Parser::currentFunction->getAllocaInst(arrayName);
                std::vector<Token> tempTokenList;
                tempTokenList.push_back(innerToken);
                llvm::Value *llvmIndex = eval(tempTokenList, *Parser::currentFunction, DATA_TYPE_INT);
                llvm::Value *indices[2] = {
                        llvm::ConstantInt::get(LLVMManager::llvmBytecodeBuilder->getInt32Ty(), llvm::APInt(32, 0)),
                        llvmIndex};
                llvm::Value *ptr = LLVMManager::llvmBytecodeBuilder->CreateInBoundsGEP(arrayAllocaInst,
                                                                                       llvm::ArrayRef<llvm::Value *>(
                                                                                               indices, 2),
                                                                                       "GetArrayElementByIndex");
                return ptr;
            }
            std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
            s.erase(end_pos, s.end());
            return currentFunction.getAllocaInst(s.substr(1));
        } else if (tokens.size() > 2 && tokens[1].text == "[" && tokens[tokens.size() - 1].text == "]") {
            std::string arrayName = tokens[0].text;
            //TODO allow expressions in the []
            Token innerToken = tokens[2];
            llvm::AllocaInst *arrayAllocaInst = Parser::currentFunction->getAllocaInst(arrayName);
            std::vector<Token> tempTokenList;
            tempTokenList.push_back(innerToken);
            llvm::Value *llvmIndex = eval(tempTokenList, *Parser::currentFunction, DATA_TYPE_INT);
            llvm::Value *indices[2] = {
                    llvm::ConstantInt::get(LLVMManager::llvmBytecodeBuilder->getInt32Ty(), llvm::APInt(32, 0)),
                    llvmIndex};
            llvm::Value *ptr = LLVMManager::llvmBytecodeBuilder->CreateInBoundsGEP(arrayAllocaInst,
                                                                                   llvm::ArrayRef<llvm::Value *>(
                                                                                           indices, 2),
                                                                                   "GetArrayElementByIndex");
            return LLVMManager::llvmBytecodeBuilder->CreateLoad(ptr, "LoadArrayPtr");
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
        std::vector<llvm::Value *> stack;
        while (!queue.empty()) {
            const auto token = queue.front();
            queue.pop_front();
            switch (token.type) {
                case TOKEN_TYPE_INTEGER_LITERAL:
                    resultType = resultType == DATA_TYPE_UNKNOWN
                                 || resultType == DATA_TYPE_BOOL
                                 ? DATA_TYPE_INT : resultType;
                    if (resultType == DATA_TYPE_FLOAT
                        || resultType == DATA_TYPE_DOUBLE) {
                        stack.push_back(llvm::ConstantFP::get(Type::getLLVMType(resultType), std::stod(token.text)));
                    } else {
                        stack.push_back(llvm::ConstantInt::get(
                                Type::getLLVMType(resultType),
                                llvm::APInt(Type::getBitCount(resultType),
                                            std::stoi(token.text))));
                    }
                    break;
                case TOKEN_TYPE_DOUBLE_LITERAL:
                    resultType = resultType == DATA_TYPE_UNKNOWN
                                 || resultType == DATA_TYPE_BOOL
                                 ? DATA_TYPE_DOUBLE : resultType;
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
                    } else if (token.text == "nullptr") {
                        variableValue = llvm::ConstantPointerNull::get(llvm::PointerType::get(
                                Type::getLLVMType(resultType == DATA_TYPE_UNKNOWN ? DATA_TYPE_BYTE : resultType,
                                                  className), 0));
                    } else {
                        variableValue = currentFunction.getValue(token.text);
                        if (variableValue == nullptr) {
                            //Check for function
                            variableValue = LLVMManager::llvmBytecodeBuilder->CreatePointerCast(LLVMManager::llvmModule->getFunction(token.text),
                                                                                                LLVMManager::llvmBytecodeBuilder->getInt8PtrTy());
                        }
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
                    } else {
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
                return LLVMManager::llvmBytecodeBuilder->CreateAdd(a, b, "addition");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFAdd(a, b, "faddition");
            }
        } else if (operatorType == "-") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateSub(a, b, "subtraction");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFSub(a, b, "fsubtraction");
            }
        } else if (operatorType == "*") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateMul(a, b, "multiplication");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFMul(a, b, "fmultiplication");
            }
        } else if (operatorType == "/") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                //TODO check if int is signed or unsigned, default to signed for now
                return LLVMManager::llvmBytecodeBuilder->CreateSDiv(a, b, "division");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFDiv(a, b, "fdivision");
            }
        }
            /**
             * LANGUAGE OPERATIONS
             * TODO SUPPORT UNSIGNED TYPES
             */
        else if (operatorType == "==") {
            if (a->getType()->isPointerTy() && !b->getType()->isPointerTy()) {
                //CAST B TO POINTER TYPE
                b = LLVMManager::llvmBytecodeBuilder->CreatePointerCast(b, a->getType(), "PointerCast");
            } else if (!a->getType()->isPointerTy() && b->getType()->isPointerTy()) {
                //CAST A TO POINTER TYPE
                a = LLVMManager::llvmBytecodeBuilder->CreatePointerCast(a, b->getType(), "PointerCast");
            }

            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(a, b, "iequalcheck");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFCmpOEQ(a, b, "fequalcheck");
            }
        } else if (operatorType == "!=") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateICmpNE(a, b, "inotequalcheck");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFCmpONE(a, b, "fnotequalcheck");
            }
        } else if (operatorType == ">") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateICmpSGT(a, b, "igreaterthancheck");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFCmpOGT(a, b, "fgreaterthancheck");
            }
        } else if (operatorType == "<") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateICmpSLT(a, b, "ilessthancheck");
            } else {
                return LLVMManager::llvmBytecodeBuilder->CreateFCmpOLT(a, b, "flessthancheck");
            }
        } else if (operatorType == "<=") {
            if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                return LLVMManager::llvmBytecodeBuilder->CreateICmpSLE(a, b, "ilessthanorequalcheck");
            }
            else {
                return LLVMManager::llvmBytecodeBuilder->CreateFCmpOLE(a, b, "flessthanorequalcheck");
            }
        } else if (operatorType == ">=") {
                if (a->getType()->isIntegerTy() && b->getType()->isIntegerTy()) {
                    return LLVMManager::llvmBytecodeBuilder->CreateICmpSGE(a, b, "igreaterthanorequalcheck");
                }
                else {
                    return LLVMManager::llvmBytecodeBuilder->CreateFCmpOGE(a, b, "fgreaterthanorequalcheck");
                }
        } else {
            return nullptr;
        }
    }

    bool MathEvaluator::isOperator(const turbolang::Token &token) {
        return token.type == TOKEN_TYPE_OPERATOR &&
               operatorPrecedenceMap.find(token.text) != operatorPrecedenceMap.end();
    }
}