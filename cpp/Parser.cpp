#include "parser/Parser.h"

namespace turbolang {

    Parser::Parser() {
        FunctionCallProcessor::load();
    }

    void Parser::parse(std::vector<token> &tokens) {
        endToken = tokens.end();
        currentToken = tokens.begin();
        //MAIN FUNCTION
        while (currentToken != endToken) {
            if (expect_function_definition()) {

            } else {
                std::cerr << "Unknown identifier " << currentToken->text << std::endl;
                currentToken++;
            }
        }
    }

    std::optional<token> Parser::expect_token_type(const tokentype &type, const std::string &name) {
        if (currentToken == endToken) {
            return std::nullopt;
        }
        if (currentToken->type != type) {
            return std::nullopt;
        }

        if (!name.empty() && currentToken->text != name) {
            return std::nullopt;
        }
        auto returnToken = currentToken;
        currentToken++;
        return *returnToken;
    }

    std::optional<token> Parser::expect_token_type(const tokentype &type) {
        return expect_token_type(type, "");
    }

    std::optional<token> Parser::expect_token() {
        if (currentToken == endToken) {
            return std::nullopt;
        }

        token returnToken = *currentToken;
        currentToken++;
        return returnToken;
    }

    std::optional<VariableType> Parser::expect_token_variable_type() {
        std::optional<token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            std::optional<int> optionalVarType = Type::getVariableType(identifier.value().text);
            if (optionalVarType.has_value()) {
               return static_cast<VariableType>(optionalVarType.value());
            }
        }
        return std::nullopt;
    }

    std::optional<FunctionType> Parser::expect_token_function_type() {
        std::optional<token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            std::optional<int> optionalFuncType = Type::getFunctionType(identifier.value().text);
            if (optionalFuncType.has_value()) {
                return static_cast<FunctionType>(optionalFuncType.value());
            }
        }
        return std::nullopt;
    }

    std::optional<bool> Parser::expect_boolean_value() {
        std::optional<token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            if (identifier.value().text == "true") {
                return true;
            } else if (identifier.value().text == "false") {
                return false;
            }
        }
        return std::nullopt;
    }

    bool Parser::expect_function_definition() {
        std::vector<token>::iterator parseStart = currentToken;
        auto functionType = expect_token_function_type();
        if (functionType.has_value()) {
            auto functionName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
            if (functionName.has_value()) {
                auto openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
                if (openingParenthesis.has_value()) {
                    Function function(functionName.value().text, functionType.value());
                    while (!expect_token_type(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                        auto parameterType = expect_token();

                        if (!parameterType.has_value()) {
                            throw std::runtime_error("Expected a type at the start of the parameter list!");
                        }
                        FunctionArgument functionArgument;
                        auto varType = Type::getVariableType(parameterType.value().text);
                        if (varType.has_value()) {
                            functionArgument.type = static_cast<VariableType>(varType.value());
                        }
                        auto variableName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
                        if (variableName.has_value()) {
                            functionArgument.name = variableName.value().text;
                        }
                        function.arguments.push_back(functionArgument);

                        if (expect_token_type(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                            break;
                        }

                        if (!expect_token_type(TOKEN_TYPE_OPERATOR, ",").has_value()) {
                            throw std::runtime_error(
                                    "Expected ',' to separate parameters or ')' to end the parameter list!");
                        }
                    }
                    function.create(functionType.value());

                    LLVMManager::llvmBytecodeBuilder->SetInsertPoint(function.entry);

                    Function::functionMap[function.name] = function;
                    currentFuncName = function.name;

                    std::optional<std::vector<Statement>> statements = parse_function_body();
                    if (function.type == FUNCTION_TYPE_VOID) {
                        LLVMManager::llvmBytecodeBuilder->CreateRet(nullptr);
                    }
                    if (!statements.has_value()) {
                        currentToken = parseStart;
                        return false;
                    }

                    return true;
                } else {
                    currentToken = parseStart;
                }
            } else {
                currentToken = parseStart;
            }
        }
        return false;
    }

    std::optional<std::vector<Statement>> Parser::parse_function_body() {
        if (!expect_token_type(TOKEN_TYPE_OPERATOR, "{").has_value()) {
            return std::nullopt;
        }

        std::vector<Statement> statements;
        std::optional<token> tempToken;
        while (true) {
            tempToken = expect_token();
            currentToken--;
            if ((tempToken.has_value() && tempToken.value().type == TOKEN_TYPE_OPERATOR &&
                 tempToken.value().text == "}")
                || !tempToken.has_value()) {
                break;
            }

            try {
                std::optional<Statement> statement = parseStatement();
                if (statement.has_value()) {
                    statements.push_back(statement.value());
                }
            }
            catch (std::exception &ex) {
                ex.what();
                break;
            }
        }

        if (!expect_token_type(TOKEN_TYPE_OPERATOR, "}").has_value()) {
            std::cout << currentToken->text << std::endl;
            throw std::runtime_error("Unbalanced '}'. Line: " + std::to_string(currentToken->lineNumber));
        }
        return statements;
    }

    std::optional<Statement> Parser::parseStatement() {
        std::vector<token>::iterator startToken;
        struct Statement statement;

        auto typeToken = expect_token_type(
                TOKEN_TYPE_IDENTIFIER);//COULD BE VARIABLE DECLARATION OR VARIABLE MODIFICATION OR FUNCTION CALL
        if (typeToken.has_value()) {
            if (Type::getVariableType(
                    typeToken.value().text).has_value()) { //TODO support custom classes as types, support strings
                //IT IS A VARIABLE DECLARATION
                parseVariableDeclaration(statement, typeToken.value());
                return statement;
            } else {
                //TODO we need the value not try optimise to get the literal  value at compile time. breakpoint at get variable by name in function definition
                //IDENTIFIER
                if (typeToken.value().text == "while") {
                    parseWhileLoop(statement);
                    return statement;
                } else if (typeToken.value().text == "return") {
                    parseReturn(statement);
                    return statement;
                } else {
                    auto equalsOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "=");
                    if (equalsOperator.has_value()) {
                        //IT IS A VARIABLE MODIFICATION. We pass in variable name
                        parseVariableModification(statement, typeToken);

                    } else {
                        //IT IS A FUNCTION CALL. We pass in the function name
                        parseFunctionCall(statement, typeToken.value().text);
                    }
                    return statement;
                }
            }
        }
        currentToken = startToken;
        return std::nullopt;
    }

    void Parser::parseVariableDeclaration(Statement &statement, const token &typeToken) {
        auto varType = Type::getVariableType(typeToken.text);
        if (varType.has_value()) {
            auto variableNameToken = expect_token_type(TOKEN_TYPE_IDENTIFIER);
            if (variableNameToken.has_value()) {
                statement.type = VARIABLE_DECLARATION;
                statement.name = variableNameToken.value().text;
                auto equalsOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "=");
                if (equalsOperator.has_value()) {
                    auto variableValueToken = expect_token();
                    if (variableValueToken.has_value()) {
                        llvm::Type *type = nullptr;
                        bool checkedForSemiColon = false;
                        llvm::Value* arraySize = nullptr;
                        switch (variableValueToken.value().type) {
                            case TOKEN_TYPE_IDENTIFIER:
                                if (variableValueToken.value().text.find('&') == 0) {
                                    //Starts with '&', its a pointer
                                    arraySize = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                                            variableValueToken.value().text.substr(1));
                                } else {
                                    arraySize = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                                            variableValueToken.value().text);
                                }
                                if (expect_token_type(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                                    //ITS A FUNCTION CALL!
                                    currentToken--;
                                    arraySize = parseFunctionCall(statement, variableValueToken.value().text);
                                    checkedForSemiColon = true;
                                } else {
                                    if (variableValueToken.value().text.find('&') == 0) {
                                        //Starts with '&', its a pointer
                                        arraySize =  Function::functionMap[currentFuncName].getAllocaInst(
                                                variableValueToken.value().text);
                                    } else {
                                        arraySize = Function::functionMap[currentFuncName].getValue(
                                                variableValueToken.value().text);
                                    }
                                }
                                type = arraySize->getType();
                                break;
                            case TOKEN_TYPE_INTEGER_LITERAL:
                                type = Type::getLLVMTypeByVariableType(varType.value());
                                arraySize = llvm::ConstantInt::get(type,
                                                                   llvm::APInt(32, std::stoi(variableValueToken.value().text)));
                                break;
                            case TOKEN_TYPE_DOUBLE_LITERAL:
                                type = Type::getLLVMTypeByVariableType(varType.value());
                                arraySize = llvm::ConstantFP::get(type,
                                                                       std::stod(variableValueToken.value().text));
                                break;
                            case TOKEN_TYPE_STRING_LITERAL:
                                arraySize = LLVMManager::llvmBytecodeBuilder->CreateGlobalString(
                                        variableValueToken.value().text, variableNameToken.value().text);
                                type = Type::getLLVMTypeByVariableType(VARIABLE_TYPE_STRING);
                                break;
                            default:
                                throw std::runtime_error("Unsupported variable value token!");
                        }
                        llvm::AllocaInst* allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type, arraySize, llvm::Twine(
                                variableNameToken.value().text));
                        llvm::Value* allocatedValue = allocaInst->getOperand(0);
                        Function::functionMap[currentFuncName].setValue(variableNameToken.value().text,
                                                                                                                                  allocatedValue);
                        Function::functionMap[currentFuncName].setAllocaInst(
                                variableNameToken.value().text, allocaInst);

                        if (!checkedForSemiColon && !expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                            throw std::runtime_error("Expected a semicolon!");
                        }
                    }
                }
            }
        }
    }


    void Parser::parseVariableModification(Statement &statement,
                                           const std::optional<token> &variableNameToken) {
        std::optional<token> variableValueToken = expect_token();
        if (variableValueToken.has_value()) {
            statement.type = VARIABLE_MODIFICATION;
            statement.name = variableNameToken.value().text;
            llvm::Type *type = nullptr;
            llvm::Value *val = nullptr;
            llvm::AllocaInst *allocaInst;
            switch (variableValueToken.value().type) {
                case TOKEN_TYPE_IDENTIFIER:
                    allocaInst = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                            variableValueToken.value().text);
                    val = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst,
                                                                       llvm::Twine(variableValueToken.value().text));
                    break;
                case TOKEN_TYPE_INTEGER_LITERAL:
                    type = Type::getLLVMTypeByVariableType(VARIABLE_TYPE_INT);
                    val = llvm::ConstantInt::get(type, std::stoi(variableValueToken.value().text), true);
                    break;
                case TOKEN_TYPE_DOUBLE_LITERAL:
                    type = Type::getLLVMTypeByVariableType(VARIABLE_TYPE_DOUBLE);
                    val = llvm::ConstantFP::get(type, std::stod(variableValueToken.value().text));
                    break;
                case TOKEN_TYPE_STRING_LITERAL:
                    val = LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(
                            variableValueToken.value().text);
                    break;
                default:
                    throw std::runtime_error("Unsupported variable value token!");
            }
            Function::functionMap[currentFuncName].setValue(variableNameToken.value().text, val);
            if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    llvm::Value *Parser::parseFunctionCall(Statement &statement,
                                           const std::string &functionName) {
        auto openingParenthesisOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesisOperator.has_value()) {
            std::vector<token> arguments;
            while (!expect_token_type(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                auto parameter = expect_token();
                if (!parameter.has_value()) {
                    break;
                }

                arguments.push_back(parameter.value());

                if (expect_token_type(TOKEN_TYPE_OPERATOR, ")").has_value()) {

                    break;
                }

                if (!expect_token_type(TOKEN_TYPE_OPERATOR, ",").has_value()) {

                    throw std::runtime_error(
                            "Expected ',' to separate parameters or ')' to end the parameter list!");
                }
            }

            if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                std::cerr << "Expected a semicolon at line " << currentToken->lineNumber << std::endl;
                return nullptr;
            }
            std::vector<llvm::Value *> llvmFunctionArguments;
            for (const auto &argument : arguments) {
                llvm::Value *llvmFunctionArgument = nullptr;
                switch (argument.type) {
                    case TOKEN_TYPE_IDENTIFIER:
                        if (argument.text.find('&') == 0) {
                            //Its a pointer
                            llvm::AllocaInst *allocaInst = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                                    argument.text.substr(1));
                            llvmFunctionArgument = allocaInst; //We want the pointer, not the value
                        } else {
                            llvm::AllocaInst *allocaInst = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                                    argument.text);
                            llvmFunctionArgument = LLVMManager::llvmBytecodeBuilder->CreateLoad(allocaInst,
                                                                                                llvm::Twine(
                                                                                                        argument.text));
                        }
                        break;
                    case TOKEN_TYPE_INTEGER_LITERAL:
                        //TODO create byte, short, long
                        llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                      llvm::APInt(32, std::stoi(argument.text)));
                        break;
                    case TOKEN_TYPE_DOUBLE_LITERAL:
                        //TODO support explicit float literals
                        llvmFunctionArgument = llvm::ConstantFP::get(Type::getLLVMTypeByVariableType(
                                VARIABLE_TYPE_DOUBLE), //TODO get type the function takes in its args
                                                                     std::stod(argument.text));
                        break;
                    case TOKEN_TYPE_STRING_LITERAL:
                        llvmFunctionArgument = LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(
                                argument.text); //TODO civ
                        break;
                    default:
                        throw std::runtime_error("Invalid token in function argument!");
                }
                if (llvmFunctionArgument != nullptr) {
                    llvmFunctionArguments.push_back(llvmFunctionArgument);
                }
            }
            return FunctionCallProcessor::process(functionName, llvmFunctionArguments);
        }
        return nullptr;
    }

    void Parser::parseWhileLoop(Statement &statement) {
        auto openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            auto booleanValue = expect_boolean_value(); //boolean literal
            if (booleanValue.has_value()) {
                auto closingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, ")");
                if (closingParenthesis.has_value()) {
                    bool value = booleanValue.value();
                    if (value) {
                        Function function = Function::functionMap[currentFuncName];
                        llvm::BasicBlock *preHeaderBasicBlock = LLVMManager::llvmBytecodeBuilder->GetInsertBlock();
                        llvm::BasicBlock *loop = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "loop",
                                                                          function.llvmFunction);
                        LLVMManager::llvmBytecodeBuilder->CreateBr(loop);
                        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(loop);

                        parse_function_body();
                        LLVMManager::llvmBytecodeBuilder->CreateBr(loop);
                        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(loop);
                        llvm::BasicBlock *loopAfterBasicBlock =
                                llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "afterloop",
                                                         function.llvmFunction);
// Insert the conditional branch into the end of LoopEndBB.
/*
                    Compiler::llvmIRBuilder.CreateCondBr(EndCond, LoopBB, AfterBB);*/

// Any new code will be inserted in AfterBB.
                        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(loopAfterBasicBlock);
                    } else {
                        auto nextToken = expect_token_type(TOKEN_TYPE_OPERATOR, "{");
                        if (nextToken.has_value()) {
                            for (nextToken = expect_token();
                                 !nextToken.has_value() || nextToken.value().type != TOKEN_TYPE_OPERATOR ||
                                 nextToken->text != "}"; nextToken = expect_token()) {
                            }
                        }
                    }
                }
            } else {
                //VARIABLE
                //TODO handle math expressions

            }
        }
    }

    void Parser::parseReturn(Statement &statement) {
        auto returnValue = expect_token();
        std::cout << "in b uildin" << std::endl;
        llvm::Value *llvmReturnValue = nullptr;
        bool checkedForSemiColon = false;
        Function targetFunction = turbolang::Function::functionMap[currentFuncName];
        if (returnValue.has_value()) {
            if (returnValue.value().type == TOKEN_TYPE_IDENTIFIER && returnValue.value().text == "void") {
                llvmReturnValue = nullptr;
                //Return void instruction in LLVM
            } else if (returnValue.value().type == TOKEN_TYPE_OPERATOR && returnValue.value().text == ";") {
                LLVMManager::llvmBytecodeBuilder->CreateRetVoid();
                return;
            } else {
                llvm::Value *allocatedValue;
                llvm::Type *type = targetFunction.llvmFunction->getReturnType();
                llvm::AllocaInst *allocaInst;
                if (returnValue.value().type == TOKEN_TYPE_IDENTIFIER) {
                    allocaInst = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                            returnValue.value().text);
                    if (expect_token_type(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                        //ITS A FUNCTION CALL!
                        currentToken--;
                        allocatedValue = parseFunctionCall(statement, returnValue.value().text);
                        checkedForSemiColon = true;
                    } else {
                        allocatedValue = turbolang::Function::functionMap[currentFuncName].getValue(
                                returnValue.value().text);
                    }
                    if (allocaInst == nullptr) {
                        allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type, 0,
                                                                                     llvm::Twine(
                                                                                             returnValue.value().text));
                    }
                    if (allocatedValue == nullptr) {
                        allocatedValue = allocaInst->getOperand(0);
                    }
                    llvmReturnValue = allocatedValue;
                } else {
                    llvmReturnValue = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                             llvm::APInt(32, std::stoi(returnValue.value().text)));
                }
            }
            LLVMManager::llvmBytecodeBuilder->CreateRet(llvmReturnValue);
        }
        if (!checkedForSemiColon && !expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
            throw std::runtime_error("Expected semicolon!");
        }
    }

}