#include "parser/parser.h"

namespace turbolang {

    parser::parser() {
        turbolang::compilermanager::functionCallProcessorMap["printf"] = new turbolang::printfcallprocessor();
        turbolang::compilermanager::functionCallProcessorMap["println"] = new turbolang::printlncallprocessor();
        turbolang::compilermanager::functionCallProcessorMap["exit"] = new turbolang::exitcallprocessor();
    }

    void parser::parse(std::vector<token> &tokens) {
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

    std::optional<token> parser::expect_token_type(const tokentype &type, const std::string &name) {
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

    std::optional<token> parser::expect_token_type(const tokentype &type) {
        return expect_token_type(type, "");
    }

    std::optional<token> parser::expect_token() {
        if (currentToken == endToken) {
            return std::nullopt;
        }

        token returnToken = *currentToken;
        currentToken++;
        return returnToken;
    }

    std::optional<variabletype> parser::expect_token_variable_type() {
        std::optional<token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            return get_variable_type(identifier.value().text);
        }
        return std::nullopt;
    }

    std::optional<functiontype> parser::expect_token_function_type() {
        std::optional<token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            return get_function_type(identifier.value().text);
        }
        return std::nullopt;
    }

    std::optional<bool> parser::expect_boolean_value() {
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

    bool parser::expect_function_definition() {
        std::vector<token>::iterator parseStart = currentToken;
        auto functionType = expect_token_function_type();
        llvm::Type *returnType = nullptr;
        switch (functionType.value()) {
            case FUNCTION_TYPE_VOID:
                returnType = llvm::Type::getVoidTy(turbolang::compilermanager::llvmContext);
                break;
            case FUNCTION_TYPE_BYTE:
                returnType = llvm::Type::getInt8Ty(turbolang::compilermanager::llvmContext);
                break;
            case FUNCTION_TYPE_SHORT:
                returnType = llvm::Type::getInt16Ty(turbolang::compilermanager::llvmContext);
                break;
            case FUNCTION_TYPE_INT:
                returnType = llvm::Type::getInt32Ty(turbolang::compilermanager::llvmContext);
                break;
            case FUNCTION_TYPE_LONG:
                returnType = llvm::Type::getInt64Ty(turbolang::compilermanager::llvmContext);
                break;
            case FUNCTION_TYPE_STRING:
                returnType = nullptr;
                //TODO
                break;
        }
        if (functionType.has_value()) {
            auto functionName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
            if (functionName.has_value()) {
                auto openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
                if (openingParenthesis.has_value()) {
                    functiondefinition functionDefinition;
                    functionDefinition.name = functionName.value().text;
                    functionDefinition.type = functionType.value();
                    while (!expect_token_type(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                        auto parameterType = expect_token();

                        if (!parameterType.has_value()) {
                            throw std::runtime_error("Expected a type at the start of the parameter list!");
                        }
                        parameterdefinition parameterDefinition;
                        auto varType = turbolang::get_variable_type(parameterType.value().text);
                        if (varType.has_value()) {
                            parameterDefinition.type = varType.value();
                        }
                        auto variableName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
                        if (variableName.has_value()) {
                            parameterDefinition.name = variableName.value().text;
                        }
                        functionDefinition.parameters.push_back(parameterDefinition);

                        if (expect_token_type(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                            break;
                        }

                        if (!expect_token_type(TOKEN_TYPE_OPERATOR, ",").has_value()) {
                            throw std::runtime_error(
                                    "Expected ',' to separate parameters or ')' to end the parameter list!");
                        }
                    }
                    std::vector<llvm::Type *> params;
                    params.reserve(functionDefinition.parameters.size());
                    llvm::Type *type = nullptr;
                    for (const parameterdefinition &param : functionDefinition.parameters) {
                        switch (param.type) {
                            case VARIABLE_TYPE_BYTE:
                                type = llvm::Type::getInt8Ty(turbolang::compilermanager::llvmContext);
                                break;
                            case VARIABLE_TYPE_SHORT:
                                type = llvm::Type::getInt16Ty(turbolang::compilermanager::llvmContext);
                                break;
                            case VARIABLE_TYPE_INT:
                                type = llvm::Type::getInt32Ty(turbolang::compilermanager::llvmContext);
                                break;
                            case VARIABLE_TYPE_LONG:
                                type = llvm::Type::getInt64Ty(turbolang::compilermanager::llvmContext);
                                break;
                            case VARIABLE_TYPE_STRING:
                                type = llvm::Type::getInt8PtrTy(turbolang::compilermanager::llvmContext, 0);
                                //TODO
                                break;
                        }

                        params.push_back(type);
                    }
                    functionDefinition.load(returnType, params);
                    compilermanager::llvmIRBuilder.SetInsertPoint(functionDefinition.entry);

                    llvm::Function *llvmFunction = turbolang::compilermanager::llvmModule->getFunction(
                            functionDefinition.name);

                    int index = 0;
                    for (auto arg = llvmFunction->arg_begin(); arg != llvmFunction->arg_end(); arg++) {
                        valueargumentwrapper argumentData;
                        argumentData.name = functionDefinition.parameters[index].name;
                        functionDefinition.arguments.push_back(argumentData);
                        index++;
                    }

                    turbolang::compilermanager::functions[functionDefinition.name] = functionDefinition;
                    currentFuncName = functionDefinition.name;

                    std::optional<std::vector<statement>> statements = parse_function_body();
                    if (functionDefinition.type == FUNCTION_TYPE_VOID) {
                        turbolang::compilermanager::llvmIRBuilder.CreateRet(nullptr);
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

    std::optional<std::vector<statement>> parser::parse_function_body() {
        if (!expect_token_type(TOKEN_TYPE_OPERATOR, "{").has_value()) {
            return std::nullopt;
        }

        std::vector<statement> statements;
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
                std::optional<statement> statement = parseStatement();
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

    std::optional<statement> parser::parseStatement() {
        std::vector<token>::iterator startToken;
        struct statement statement;

        auto typeToken = expect_token_type(
                TOKEN_TYPE_IDENTIFIER);//COULD BE VARIABLE DECLARATION OR VARIABLE MODIFICATION OR FUNCTION CALL
        if (typeToken.has_value()) {
            if (get_variable_type(
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

    void parser::parseVariableDeclaration(statement &statement, const token &typeToken) {
        auto varType = get_variable_type(typeToken.text);
        if (varType.has_value()) {
            auto variableNameToken = expect_token_type(TOKEN_TYPE_IDENTIFIER);
            if (variableNameToken.has_value()) {
                statement.type = VARIABLE_DECLARATION;
                statement.vartype = varType.value();
                statement.name = variableNameToken.value().text;
                auto equalsOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "=");
                if (equalsOperator.has_value()) {
                    auto variableValueToken = expect_token();
                    if (variableValueToken.has_value()) {
                        llvm::Type *type = nullptr;
                        auto twine = llvm::Twine(variableNameToken.value().text);
                        llvm::AllocaInst *allocaInst = nullptr;
                        llvm::Constant *arraySize;
                        llvm::Value *allocatedValue = nullptr;
                        bool checkedForSemiColon = false;
                        switch (variableValueToken.value().type) {
                            case TOKEN_TYPE_IDENTIFIER:
                                allocaInst = turbolang::compilermanager::functions[currentFuncName].get_alloca_instance(
                                        variableValueToken.value().text);
                                if (expect_token_type(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                                    //ITS A FUNCTION CALL!
                                    currentToken--;
                                    allocatedValue = parseFunctionCall(statement, variableValueToken.value().text);
                                    checkedForSemiColon = true;
                                } else {
                                    allocatedValue = turbolang::compilermanager::functions[currentFuncName].get_variable_by_name(
                                            variableValueToken.value().text);
                                }
                                type = allocatedValue->getType();
                                arraySize = llvm::ConstantInt::get(type, llvm::APInt(32, 9)); //civ?????????h elp
                                //TODO get allocatedValue value
                                break;
                            case TOKEN_TYPE_INTEGER_LITERAL:
                                if (varType.value() == VARIABLE_TYPE_BYTE) {
                                    type = llvm::Type::getInt8Ty(turbolang::compilermanager::llvmContext);
                                    arraySize = llvm::ConstantInt::get(
                                            type,
                                            llvm::APInt(8, std::stoi(variableValueToken.value().text)));
                                } else if (varType.value() == VARIABLE_TYPE_SHORT) {
                                    type = llvm::Type::getInt16Ty(turbolang::compilermanager::llvmContext);
                                    arraySize = llvm::ConstantInt::get(
                                            type,
                                            llvm::APInt(16, std::stoi(variableValueToken.value().text)));
                                } else if (varType.value() == VARIABLE_TYPE_INT) {
                                    type = llvm::Type::getInt32Ty(turbolang::compilermanager::llvmContext);
                                    arraySize = llvm::ConstantInt::get(
                                            type,
                                            llvm::APInt(32, std::stoi(variableValueToken.value().text)));
                                } else if (varType.value() == VARIABLE_TYPE_LONG) {
                                    type = llvm::Type::getInt64Ty(turbolang::compilermanager::llvmContext);
                                    arraySize = llvm::ConstantInt::get(
                                            type,
                                            llvm::APInt(64, std::stol(variableValueToken.value().text)));
                                }
                                break;
                            case TOKEN_TYPE_DOUBLE_LITERAL:
                                if (varType.value() == VARIABLE_TYPE_DOUBLE) {
                                    type = llvm::IntegerType::getDoubleTy(turbolang::compilermanager::llvmContext);
                                    arraySize = llvm::ConstantFP::get(type,
                                                                      std::stod(variableValueToken.value().text));
                                } else if (varType.value() == VARIABLE_TYPE_FLOAT) {
                                    type = llvm::IntegerType::getFloatTy(turbolang::compilermanager::llvmContext);
                                    arraySize = llvm::ConstantFP::get(type, llvm::APFloat(
                                            std::stof(variableValueToken.value().text)));
                                } else {
                                    //????
                                }
                                break;
                            case TOKEN_TYPE_STRING_LITERAL:
                                allocatedValue = turbolang::compilermanager::llvmIRBuilder.CreateGlobalString(
                                        variableValueToken.value().text, variableNameToken.value().text);
                                type = allocatedValue->getType();
                                arraySize = llvm::ConstantInt::get(type, llvm::APInt(32, 9)); //civ?????????h elp
                                break;
                            default:
                                throw std::runtime_error("Unsupported variable value token!");
                        }
                        if (allocaInst == nullptr) {
                            allocaInst = turbolang::compilermanager::llvmIRBuilder.CreateAlloca(type, arraySize, twine);
                        }
                        if (allocatedValue == nullptr) {
                            allocatedValue = allocaInst->getOperand(0);
                        }
                        turbolang::compilermanager::functions[currentFuncName].set_variable_by_name(
                                variableNameToken.value().text, allocatedValue);
                        turbolang::compilermanager::functions[currentFuncName].set_alloca_instance(
                                variableNameToken.value().text, allocaInst);
                        if (!checkedForSemiColon && !expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                            throw std::runtime_error("Expected a semicolon!");
                        }
                    }
                }
            }
        }
    }


    void parser::parseVariableModification(statement &statement,
                                           const std::optional<token> &variableNameToken) {
        std::optional<token> variableValueToken = expect_token();
        if (variableValueToken.has_value()) {
            statement.type = VARIABLE_MODIFICATION;
            //statement.vartype = ?
            statement.name = variableNameToken.value().text;
            llvm::Type *type = nullptr;
            llvm::Constant *arraySize = nullptr;
            llvm::Value *val = nullptr;
            switch (variableValueToken.value().type) {
                case TOKEN_TYPE_IDENTIFIER:
                    type = turbolang::compilermanager::functions[currentFuncName].get_variable_by_name(
                            variableValueToken.value().text)->getType();
                    val = turbolang::compilermanager::functions[currentFuncName].get_variable_by_name(
                            variableValueToken.value().text);
                    break;
                case TOKEN_TYPE_INTEGER_LITERAL:
                    type = llvm::Type::getInt32Ty(turbolang::compilermanager::llvmContext);
                    arraySize = llvm::ConstantInt::get(
                            type,
                            llvm::APInt(32, std::stoi(variableValueToken.value().text)));
                    break;
                case TOKEN_TYPE_DOUBLE_LITERAL:
                    type = llvm::Type::getDoubleTy(turbolang::compilermanager::llvmContext);
                    arraySize = llvm::ConstantFP::get(
                            type,
                            llvm::APFloat(std::stod(variableValueToken.value().text)));
                    break;
                case TOKEN_TYPE_STRING_LITERAL:
                    arraySize = turbolang::compilermanager::llvmIRBuilder.CreateGlobalStringPtr(
                            variableValueToken.value().text);
                    break;
                default:
                    throw std::runtime_error("Unsupported variable value token!");
            }
            auto allocaInst = turbolang::compilermanager::functions[currentFuncName].get_alloca_instance(
                    variableNameToken.value().text);
            if (val == nullptr) {
                auto storeInst = turbolang::compilermanager::llvmIRBuilder.CreateStore(
                        arraySize, allocaInst,
                        false);

                val = storeInst->getOperand(0);
            }
            turbolang::compilermanager::functions[currentFuncName].set_variable_by_name(variableNameToken.value().text,
                                                                                        val);
            if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    llvm::Value *parser::parseFunctionCall(statement &statement,
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
            }
            functioncallprocessor *funcCallProcessor = turbolang::compilermanager::functionCallProcessorMap[functionName];
            turbolang::functiondefinition targetFunction =
                    funcCallProcessor == nullptr ? turbolang::compilermanager::functions[functionName] :
                    turbolang::compilermanager::functions[currentFuncName];
            llvm::Function *function = turbolang::compilermanager::llvmModule->getFunction(targetFunction.name);
            int index = 0;
            for (auto arg = function->arg_begin(); arg != function->arg_end(); arg++) {
                if (auto *argValue = llvm::dyn_cast<llvm::Value>(arg)) {
                    targetFunction.arguments[index].value = argValue;
                    /*std::cout << targetFunction.arguments[index].name << " is name we are modifying value of "
                              << std::endl;*/
                }
                index++;
            }
            if (funcCallProcessor == nullptr) {
                turbolang::compilermanager::functions[functionName] = targetFunction;
            } else {
                turbolang::compilermanager::functions[currentFuncName] = targetFunction;
            }
            std::vector<llvm::Value *> llvmFunctionArguments;
            for (const auto &argument : arguments) {
                llvm::Value *llvmFunctionArgument = nullptr;
                switch (argument.type) {
                    case TOKEN_TYPE_IDENTIFIER:
                        llvmFunctionArgument = turbolang::compilermanager::functions[currentFuncName].get_variable_by_name(
                                argument.text);//TODO
                        break;
                    case TOKEN_TYPE_INTEGER_LITERAL:
                        //TODO create byte, short, long
                        llvmFunctionArgument = llvm::ConstantInt::get(turbolang::compilermanager::llvmContext,
                                                                      llvm::APInt(32, std::stoi(argument.text)));
                        break;
                    case TOKEN_TYPE_DOUBLE_LITERAL:
                        //TODO support explicit float literals
                        llvmFunctionArgument = llvm::ConstantFP::get(turbolang::compilermanager::llvmContext,
                                                                     llvm::APFloat(std::stod(argument.text)));
                        break;
                    case TOKEN_TYPE_STRING_LITERAL:
                        llvmFunctionArgument = turbolang::compilermanager::llvmIRBuilder.CreateGlobalStringPtr(
                                argument.text);
                        break;
                    default:
                        throw std::runtime_error("Invalid token in function argument!");
                }
                if (llvmFunctionArgument != nullptr) {
                    llvmFunctionArguments.push_back(llvmFunctionArgument);
                }
            }
            //was here
            if (funcCallProcessor == nullptr) {
                return turbolang::functioncallprocessor::external_func_process(targetFunction,
                                                                               llvmFunctionArguments);
            } else {
                return funcCallProcessor->process(targetFunction, llvmFunctionArguments);
            }
        }
        return nullptr;
    }

    void parser::parseWhileLoop(statement &statement) {
        auto openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            auto booleanValue = expect_boolean_value(); //boolean literal
            if (booleanValue.has_value()) {
                auto closingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, ")");
                if (closingParenthesis.has_value()) {
                    bool value = booleanValue.value();
                    if (value) {
                        struct functiondefinition function = compilermanager::functions[currentFuncName];
                        llvm::BasicBlock *preHeaderBasicBlock = compilermanager::llvmIRBuilder.GetInsertBlock();
                        llvm::BasicBlock *loop = llvm::BasicBlock::Create(compilermanager::llvmContext, "loop",
                                                                          function.llvmFunction);
                        std::cout << "second?" << std::endl;
                        compilermanager::llvmIRBuilder.CreateBr(loop);
                        compilermanager::llvmIRBuilder.SetInsertPoint(loop);

                        parse_function_body();
                        compilermanager::llvmIRBuilder.CreateBr(loop);
                        compilermanager::llvmIRBuilder.SetInsertPoint(loop);
                        llvm::BasicBlock *loopAfterBasicBlock =
                                llvm::BasicBlock::Create(turbolang::compilermanager::llvmContext, "afterloop",
                                                         function.llvmFunction);
// Insert the conditional branch into the end of LoopEndBB.
/*
                    compilermanager::llvmIRBuilder.CreateCondBr(EndCond, LoopBB, AfterBB);*/

// Any new code will be inserted in AfterBB.
                        compilermanager::llvmIRBuilder.SetInsertPoint(loopAfterBasicBlock);
                    }
                    else {
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

    void parser::parseReturn(statement &statement) {
        auto returnValue = expect_token();
        llvm::Value *llvmReturnValue = nullptr;
        bool checkedForSemiColon = false;
        int index = 0;
        turbolang::functiondefinition *targetFunction = &turbolang::compilermanager::functions[currentFuncName];
        llvm::Function *function = targetFunction->llvmFunction;
        for (auto arg = function->arg_begin(); arg != function->arg_end(); arg++) {
            if (auto *argValue = llvm::dyn_cast<llvm::Value>(arg)) {
                targetFunction->arguments[index].value = argValue;
                /*std::cout << targetFunction->arguments[index].name << " is name we are modifying value of "
                          << std::endl;*/
            }
            index++;
        }
        if (returnValue.has_value()) {
            if (returnValue.value().type == TOKEN_TYPE_IDENTIFIER && returnValue.value().text == "void") {
                llvmReturnValue = nullptr;
                //Return void instruction in LLVM
            } else if (returnValue.value().type == TOKEN_TYPE_OPERATOR && returnValue.value().text == ";") {
                turbolang::compilermanager::llvmIRBuilder.CreateRetVoid();
                return;
            } else {
                llvm::Value *allocatedValue;
                llvm::Type *type = turbolang::compilermanager::llvmModule->getFunction(
                        currentFuncName)->getReturnType();
                llvm::AllocaInst *allocaInst;
                llvm::Constant *arraySize = llvm::ConstantInt::get(type, llvm::APInt(32, 0));
                if (returnValue.value().type == TOKEN_TYPE_IDENTIFIER) {
                    allocaInst = turbolang::compilermanager::functions[currentFuncName].get_alloca_instance(
                            returnValue.value().text);
                    if (expect_token_type(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                        //ITS A FUNCTION CALL!
                        currentToken--;
                        allocatedValue = parseFunctionCall(statement, returnValue.value().text);
                        checkedForSemiColon = true;
                    } else {
                        allocatedValue = turbolang::compilermanager::functions[currentFuncName].get_variable_by_name(
                                returnValue.value().text);
                    }
                    if (allocaInst == nullptr) {
                        allocaInst = turbolang::compilermanager::llvmIRBuilder.CreateAlloca(type, arraySize,
                                                                                            llvm::Twine(
                                                                                                    returnValue.value().text));
                    }
                    if (allocatedValue == nullptr) {
                        allocatedValue = allocaInst->getOperand(0);
                    }
                    llvmReturnValue = allocatedValue;
                } else {
                    llvmReturnValue = llvm::ConstantInt::get(turbolang::compilermanager::llvmContext,
                                                             llvm::APInt(32, std::stoi(returnValue.value().text)));
                }
            }
            turbolang::compilermanager::llvmIRBuilder.CreateRet(llvmReturnValue);
        }
        if (!checkedForSemiColon && !expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
            throw std::runtime_error("Expected semicolon!");
        }
    }
}