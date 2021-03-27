#include "parser/Parser.h"

namespace turbolang {
    std::vector<Token>::iterator Parser::currentToken;
    std::vector<Token>::iterator Parser::endToken;
    std::string Parser::currentFuncName;

    void Parser::parse(std::vector<Token> &tokens) {
        endToken = tokens.end();
        currentToken = tokens.begin();
        if (currentToken->type == TOKEN_TYPE_IDENTIFIER && currentToken->text == "import") {
            parseImportStatement();
        }
        //MAIN FUNCTION
        while (currentToken != endToken) {
            if (currentToken->type == TOKEN_TYPE_IDENTIFIER) {
                if (currentToken->text == "declare") {
                    currentToken++;
                    parseFunctionDeclareStatement();
                    continue;
                } else if (currentToken->text == "class") {
                    currentToken++;
                    parseClassDefinition();
                    continue;
                }
            }
            if (expectFunctionDefinition()) {

            } else {
                std::cerr << "Unknown identifier " << currentToken->text << std::endl;
                currentToken++;
            }
        }
    }

    void Parser::parseClassDefinition() {
        auto nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (nextToken.has_value()) {
            std::string className = nextToken.value().text;
            std::cout << "CLASS NAME: " << className << std::endl;
            parseFunctionBody();
            exit(0);
        }
    }

    void Parser::parseFunctionDeclareStatement() {
        auto nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (nextToken.has_value()) {
            auto functionTypeOptional = Type::getType(nextToken.value().text);
            if (functionTypeOptional.has_value()) {
                auto functionType = functionTypeOptional.value();
                nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                if (nextToken.has_value()) {
                    std::string functionName = nextToken.value().text;
                    bool hasArguments = !expectTokenType(TOKEN_TYPE_OPERATOR, "()").has_value();
                    if (!hasArguments || expectTokenType(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                        bool expectingComma = false;
                        std::vector<llvm::Type *> functionArgumentTypes;
                        bool argumentSizeConstant = true;
                        if (hasArguments && !expectTokenType(TOKEN_TYPE_OPERATOR, ")")) {
                            while (true) {
                                if (expectingComma) {
                                    nextToken = expectToken();
                                    if (nextToken.has_value()) {
                                        if (nextToken.value().type == TOKEN_TYPE_OPERATOR) {
                                            if (nextToken.value().text == ")") {
                                                break;
                                            } else if (nextToken.value().text == ",") {
                                                expectingComma = false;
                                                continue;
                                            }
                                        }
                                    }
                                    throw std::runtime_error(
                                            "Please separate function declaration arguments with commas! Do NOT specify a name for the types in the arguments!");
                                } else {
                                    nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                                    if (nextToken.has_value()) {
                                        functionTypeOptional = Type::getType(nextToken.value().text);
                                        if (functionTypeOptional.has_value()) {
                                            auto functionArgumentType = functionTypeOptional.value();
                                            functionArgumentTypes.push_back(
                                                    Type::getLLVMType(functionArgumentType));
                                            expectingComma = true;
                                            continue;
                                        } else {

                                        }
                                    } else {
                                        argumentSizeConstant = false;
                                        for (int i = 0; i < 3; i++) {
                                            nextToken = expectToken();
                                            if (!nextToken.has_value() || nextToken.value().text != ".") {
                                                argumentSizeConstant = true;
                                                //Invalid type?
                                                break;
                                            }
                                        }
                                        if (!argumentSizeConstant) {
                                            expectingComma = true;
                                            continue;
                                        }
                                    }
                                    throw std::runtime_error(
                                            "Invalid function argument type: " + nextToken.value().text);
                                }
                            }
                        }
                        nextToken = expectTokenType(TOKEN_TYPE_OPERATOR, ";");
                        if (nextToken.has_value()) {
                            FunctionInternal::defineFunction(functionName,
                                                             Type::getLLVMType(functionType),
                                                             functionArgumentTypes, argumentSizeConstant);
                        } else {
                            throw std::runtime_error("Expected semicolon!");
                        }
                    }
                }
            }
        }
    }

    void Parser::parseImportStatement() {
        std::vector<std::string> importedModules;
        bool importPending = false;
        while (true) {
            auto nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER, "import");
            if (nextToken.has_value()) {
                importPending = true;
            } else {
                if (importPending) {
                    nextToken = expectTokenType(TOKEN_TYPE_STRING_LITERAL);
                    if (nextToken.has_value()) {
                        importedModules.push_back(nextToken.value().text);
                        importPending = false;
                    } else {
                        throw std::runtime_error("Invalid module format!");
                    }
                } else {
                    break;
                }
            }
        }

        for (std::string &module : importedModules) {
            if (module.substr(module.length() - 2) != "tl") {
                module = module + ".tl";
            }
            auto basePath = std::filesystem::current_path();
            auto modulesPath = basePath.string() + "/build/modules";
            std::filesystem::current_path(modulesPath);
            std::string code;
            std::ifstream sourceFile(module);
            if (!SourceCodeReader::readAndFilterCode(sourceFile, &code)) {
                throw std::runtime_error("Failed to read module: " + modulesPath + "/" + module);
            }
            std::filesystem::current_path(basePath);
            std::vector<Token> tokens = Tokenizer::tokenize(code);
            auto prevCurrToken = currentToken;
            auto prevEndToken = endToken;
            auto prevCurrFuncName = currentFuncName;
            Parser::parse(tokens);
            currentToken = prevCurrToken;
            endToken = prevEndToken;
            currentFuncName = prevCurrFuncName;

        }
    }

    llvm::Value *Parser::expectExpression(const Token *token, const std::string &endAtStr,
                                          const std::function<void(std::vector<Token> &)> &extraProcessing) {
        auto beforeFunctionCallCheck = currentToken;
        auto functionNameToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (functionNameToken.has_value()) {
            auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
            if (openingParenthesis.has_value()) {
                return Parser::parseFunctionCall(functionNameToken.value().text);
            }
        }
        currentToken = beforeFunctionCallCheck;
        std::optional<Token> nextToken;
        std::vector<Token> expressionTokens;
        if (token != nullptr) {
            expressionTokens.push_back(*token);
        }
        std::optional<Token> tempToken;
        while (true) {
            nextToken = expectToken();
            if (nextToken.has_value() && nextToken.value().text != endAtStr) {
                expressionTokens.push_back(nextToken.value());
            } else {
                break;
            }
        }
        extraProcessing(expressionTokens);
        return MathEvaluator::eval(expressionTokens, Function::functionMap[currentFuncName]);
    }

    std::optional<Token> Parser::expectTokenType(const TokenType &type, const std::string &name) {
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

    std::optional<Token> Parser::expectTokenType(const TokenType &type) {
        return expectTokenType(type, "");
    }

    std::optional<Token> Parser::expectToken() {
        if (currentToken == endToken) {
            return std::nullopt;
        }
        auto returnToken = currentToken;
        currentToken++;
        return *returnToken;
    }

    std::optional<DataType> Parser::expectTokenVariableType() {
        std::optional<Token> identifier = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            std::optional<DataType> optionalVarType = Type::getType(identifier.value().text);
            if (optionalVarType.has_value() && optionalVarType.value() != DATA_TYPE_VOID) {
                return optionalVarType.value();
            }
        }
        return std::nullopt;
    }

    std::optional<DataType> Parser::expectTokenFunctionType() {
        std::optional<Token> identifier = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            std::optional<DataType> optionalFuncType = Type::getType(identifier.value().text);
            if (optionalFuncType.has_value()) {
                return optionalFuncType.value();
            }
        }
        return std::nullopt;
    }

    bool Parser::expectFunctionDefinition() {
        auto parseStart = currentToken;
        //Function return type
        auto returnType = expectTokenFunctionType();
        if (returnType.has_value()) {
            //Function name
            auto functionName = expectTokenType(TOKEN_TYPE_IDENTIFIER);
            if (functionName.has_value()) {
                //Opening parenthesis for the function arguments
                bool hasArguments = !expectTokenType(TOKEN_TYPE_OPERATOR, "()").has_value();
                if (hasArguments) {
                    currentToken++;
                }
                Function function(functionName.value().text, returnType.value());
                while (hasArguments && !expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                    auto parameterType = expectToken();
                    if (!parameterType.has_value()) {
                        throw std::runtime_error("Expected a type at the start of the parameter list!");
                    }
                    //Function parameter
                    FunctionArgument functionArgument;
                    //Parameter type
                    auto varType = Type::getType(parameterType.value().text);
                    if (varType.has_value()) {
                        functionArgument.type = varType.value();
                    }
                    //Parameter name
                    auto variableName = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                    if (variableName.has_value()) {
                        functionArgument.name = variableName.value().text;
                    }
                    function.arguments.push_back(functionArgument);

                    //Check if we reached the end.
                    if (expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                        break;
                    }

                    //Check if they separated the arguments with a comma.
                    if (!expectTokenType(TOKEN_TYPE_OPERATOR, ",").has_value()) {
                        throw std::runtime_error(
                                "Expected ',' to separate parameters or ')' to end the parameter list!");
                    }
                }
                //Create the function.
                function.create();

                //Store the function in the map for fast access by name.
                Function::functionMap[function.name] = function;
                currentFuncName = function.name;
                try {
                    //Parse the functions body.
                    parseFunctionBody();
                }
                catch (std::exception &ex) {
                    ex.what();
                    currentToken = parseStart;
                    return false;
                }
                if (function.type == DATA_TYPE_VOID) {
                    LLVMManager::llvmBytecodeBuilder->CreateRet(nullptr);
                }
                return true;
            } else {
                currentToken = parseStart;
            }
        } else {
            currentToken = parseStart;
        }
        return false;
    }


    void Parser::parseFunctionBody() {
        //Opening curly brackets of the function.
        if (expectTokenType(TOKEN_TYPE_OPERATOR, "{").has_value()) {
            std::optional<Token> tempToken;
            while (true) {
                //Check if we have approached the end of the function.
                tempToken = expectTokenType(TOKEN_TYPE_OPERATOR, "}");
                if (tempToken.has_value()) {
                    //We can quit processing the body as we have reached the end.
                    break;
                } else {
                    try {
                        parseStatement();
                        //Finished processing a statement. We are expecting the function body to end here.
                        if (expectTokenType(TOKEN_TYPE_OPERATOR, "}").has_value()) {
                            return;
                        } else {
                            //Why didn't the function body end?
                            throw std::runtime_error(
                                    "Unbalanced '}'. Line: " + std::to_string(currentToken->lineNumber));
                        }
                    }
                    catch (std::exception &ex) {
                        ex.what();
                    }
                }
            }
        }
    }

    void Parser::parseStatement() {
        std::vector<Token>::iterator startToken;
        auto token = expectToken();
        if (token.has_value()) {
            switch (token.value().type) {
                case TOKEN_TYPE_WHILE:
                    parseWhileLoop();
                    break;
                case TOKEN_TYPE_RETURN:
                    parseReturnStatement();
                    break;
                case TOKEN_TYPE_IF:
                    parseIfStatement();
                    break;
                case TOKEN_TYPE_IDENTIFIER:
                    //Is it a valid data type?
                    if (Type::getType(
                            token.value().text).has_value()) { //TODO support custom classes as types, support strings
                        //Then a variable is for sure being declared/created.
                        parseVariableDeclaration(token.value());
                    } else {
                        //Is the next token an operator?
                        auto operatorToken = expectTokenType(TOKEN_TYPE_OPERATOR);
                        if (operatorToken.has_value()) {
                            //Is it an assignment? If yes we are modifying a variable.
                            if (operatorToken.value().text == "=") {
                                parseVariableModification(token);
                            }
                                //Is it an opening parenthesis? If yes we are calling a function.
                            else if (operatorToken.value().text == "(") {
                                parseFunctionCall(token.value().text);
                                if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                                    throw std::runtime_error(
                                            &"Expected a semicolon at line "[currentToken->lineNumber]);
                                }
                            } else {
                                throw std::runtime_error(
                                        "Unexpected operator token. Expected an assignment operator or an opening parenthesis! Line: " +
                                        std::to_string(currentToken->lineNumber));
                            }
                        } else {
                            throw std::runtime_error(
                                    "Unexpected token. Expected an assignment operator or an opening parenthesis! Line: " +
                                    std::to_string(currentToken->lineNumber));
                        }

                    }
                    break;
                default:
                    throw std::runtime_error(
                            "Failed to parse a statement. A statement cannot start with such a token. Line: " +
                            std::to_string(currentToken->lineNumber));
                    break;
            }
        } else {
            throw std::runtime_error(
                    "Failed to parse a statement. No token found? Line: " + std::to_string(currentToken->lineNumber));
        }
    }

    void Parser::parseVariableDeclaration(const Token &typeToken) {
        //Is the declared variable an array.
        bool isArray = false;
        //If the variable is an array, we will store its size in this variable.
        int arraySize;
        //What type of variable is it?
        auto varType = Type::getType(typeToken.text);
        if (varType.has_value()) {
            //We will store the variable name.
            std::string varName;
            auto variableNameOrOpeningBracketToken = expectToken();
            if (variableNameOrOpeningBracketToken.has_value()) {
                if (variableNameOrOpeningBracketToken.value().text == "[") {
                    isArray = true;
                    auto arraySizeLiteralToken = expectTokenType(TOKEN_TYPE_INTEGER_LITERAL);
                    if (arraySizeLiteralToken.has_value()) {
                        auto closingBracketToken = expectTokenType(TOKEN_TYPE_OPERATOR, "]");
                        if (closingBracketToken.has_value()) {
                            auto varNameToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                            if (varNameToken.has_value()) {
                                arraySize = std::stoi(arraySizeLiteralToken.value().text);
                                varName = varNameToken.value().text;
                            }
                        }
                    }
                } else {
                    varName = variableNameOrOpeningBracketToken.value().text;
                }

                llvm::Type *type = Type::getLLVMType(varType.value());
                llvm::AllocaInst *allocaInst;
                if (isArray) {
                    allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type,
                                                                                llvm::ConstantInt::get(
                                                                                        Type::getLLVMType(
                                                                                                DATA_TYPE_INT),
                                                                                        llvm::APInt(32, arraySize)),
                                                                                llvm::Twine(varName));
                } else {
                    allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type, nullptr,
                                                                                llvm::Twine(
                                                                                        varName));
                }
                Function::functionMap[currentFuncName].setAllocaInst(
                        varName, allocaInst);

                auto equalsOperator = expectTokenType(TOKEN_TYPE_OPERATOR, "=");
                //bool signedType = typeToken.text.find('u') != 0;
                llvm::Value *allocatedValue = nullptr;
                //Check if we are also allocating and not just declaring the variable.
                if (equalsOperator.has_value()) {
                    //Handle an expression.
                    allocatedValue = expectExpression();

                    if (allocatedValue == nullptr) {
                        allocatedValue = allocaInst->getOperand(0);
                    }

                    Function::functionMap[currentFuncName].setValue(varName, allocatedValue);
                }

                if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                    throw std::runtime_error("Expected a semicolon!");
                }
            }
        }
    }


    void Parser::parseVariableModification(const std::optional<Token> &variableNameToken) {
        std::optional<Token> variableValueToken = expectToken();
        if (variableValueToken.has_value()) {
            llvm::Value *val = nullptr;
            auto prevTokenState = currentToken;
            //TODO abstract the SINGLE VALUE to a function called llvm::Value* expectSingleValue();
            if (variableValueToken.has_value()) {
                llvm::AllocaInst *allocaInst = Function::functionMap[currentFuncName].getAllocaInst(
                        variableNameToken.value().text);
                //Handle an expression
                val = expectExpression(&variableValueToken.value());
            }
            Function::functionMap[currentFuncName].setValue(variableNameToken.value().text, val);
            if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    llvm::Value *Parser::parseFunctionCall(const std::string &functionName) {
        std::vector<Token> arguments;
        while (!expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
            auto parameter = expectToken();
            if (!parameter.has_value()) {
                break;
            }

            arguments.push_back(parameter.value());

            if (expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {

                break;
            }

            if (!expectTokenType(TOKEN_TYPE_OPERATOR, ",").has_value()) {
                throw std::runtime_error(
                        "Expected ',' to separate parameters or ')' to end the parameter list!");
            }
        }
        std::vector<llvm::Value *> llvmFunctionArguments;
        for (const auto &argument : arguments) {
            llvm::Value *llvmFunctionArgument;
            switch (argument.type) {
                case TOKEN_TYPE_IDENTIFIER:
                    if (argument.text.find('&') == 0) {
                        //Its a pointer
                        llvm::AllocaInst *allocaInst = turbolang::Function::functionMap[currentFuncName].getAllocaInst(
                                argument.text.substr(1));
                        llvmFunctionArgument = allocaInst; //We want the pointer, not the value
                    } else {
                        llvmFunctionArgument = Function::functionMap[currentFuncName].getValue(argument.text);
                    }
                    break;
                case TOKEN_TYPE_INTEGER_LITERAL:
                    for (const auto &functionArg : Function::functionMap[functionName].arguments) {
                        switch (functionArg.type) {
                            case DATA_TYPE_BYTE:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(8, std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_UBYTE:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(8, std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_SHORT:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(16,
                                                                                          std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_USHORT:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(16,
                                                                                          std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_INT:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(32,
                                                                                          std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_UINT:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(32,
                                                                                          std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_LONG:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(64,
                                                                                          std::stoi(argument.text)));
                                break;
                            case DATA_TYPE_ULONG:
                                llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                              llvm::APInt(64,
                                                                                          std::stoi(argument.text)));
                                break;
                            default:
                                //TODO throw exception
                                break;
                        }
                    }
                    break;
                case TOKEN_TYPE_DOUBLE_LITERAL:
                    for (const auto &functionArg : Function::functionMap[functionName].arguments) {
                        switch (functionArg.type) {
                            case DATA_TYPE_FLOAT:
                                llvmFunctionArgument = llvm::ConstantFP::get(Type::getLLVMType(DATA_TYPE_FLOAT),
                                                                             std::stod(argument.text));
                                break;
                            case DATA_TYPE_DOUBLE:
                                llvmFunctionArgument = llvm::ConstantFP::get(Type::getLLVMType(DATA_TYPE_DOUBLE),
                                                                             std::stod(argument.text));
                                break;
                            default:
                                //TODO throw exception
                                break;
                        }
                    }
                    break;
                case TOKEN_TYPE_STRING_LITERAL:
                    llvmFunctionArgument = LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(
                            argument.text); //TODO civ
                    break;
                default:
                    throw std::runtime_error("Invalid Token in function argument!");
            }
            if (llvmFunctionArgument != nullptr) {
                llvmFunctionArguments.push_back(llvmFunctionArgument);
            }
        }
        return FunctionCallProcessor::process(functionName, llvmFunctionArguments);
    }

    void Parser::parseWhileLoop() {
        auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            auto loopCheckState = currentToken;
            auto extraProcessing = [](std::vector<Token> &tokens) {
                tokens.pop_back();
            };
            llvm::Value *whileLoopValue = expectExpression(nullptr, "{", extraProcessing);
            Function *function = &Function::functionMap[currentFuncName];
            llvm::BasicBlock *loop = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "loop",
                                                              function->llvmFunction);

            llvm::BasicBlock *afterLoop =
                    llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "afterloop",
                                             function->llvmFunction);

            auto comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(whileLoopValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, loop, afterLoop);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(loop);
            currentToken--;
            parseFunctionBody();
            auto prevCurrentTokenState = currentToken;
            currentToken = loopCheckState;
            whileLoopValue = expectExpression(nullptr, "{", extraProcessing);
            currentToken = prevCurrentTokenState;
            comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(whileLoopValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));

            LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, loop, afterLoop);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterLoop);
        }
    }

    void Parser::parseReturnStatement() {
        llvm::Value *llvmReturnValue;
        auto returnValue = expectToken();
        if (returnValue.has_value()) {
            if (returnValue.value().type == TOKEN_TYPE_IDENTIFIER && returnValue.value().text == "void") {
                llvmReturnValue = nullptr;
                //Return void instruction in LLVM
            } else if (returnValue.value().type == TOKEN_TYPE_OPERATOR && returnValue.value().text == ";") {
                llvmReturnValue = nullptr;
                return;
            } else {
                llvmReturnValue = expectExpression(&returnValue.value());
            }
        } else {
            throw std::runtime_error(&"Invalid return statement! Line: "[currentToken->lineNumber]);
        }
        LLVMManager::llvmBytecodeBuilder->CreateRet(llvmReturnValue);
        if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
            throw std::runtime_error("Expected semicolon!");
        }
    }

    void Parser::parseIfStatement() {
        auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            auto extraProcessing = [](std::vector<Token> &tokens) {
                tokens.pop_back();
            };
            llvm::Value *statementValue = expectExpression(nullptr, "{", extraProcessing);
            Function *function = &Function::functionMap[currentFuncName];
            llvm::BasicBlock *statement = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "if-statement",
                                                                   function->llvmFunction);

            llvm::BasicBlock *afterStatement =
                    llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "after-if-statement",
                                             function->llvmFunction);

            auto comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(statementValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, statement, afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(statement);
            currentToken--;
            parseFunctionBody();
            LLVMManager::llvmBytecodeBuilder->CreateBr(afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterStatement);
            auto beforeCheck = currentToken;
            while (true) {
                auto elseIfToken = expectTokenType(TOKEN_TYPE_ELSE_IF);
                if (elseIfToken.has_value()) {
                    parseElifStatement(comp);
                    beforeCheck = currentToken;
                    //Check if an else statement exists...
                    auto elseToken = expectTokenType(TOKEN_TYPE_ELSE);
                    if (elseToken.has_value()) {
                        parseElseStatement(comp);
                        break;
                    } else {
                        //No else statement, we are done...
                        //currentToken = beforeCheck;
                        continue;
                    }
                } else {
                    currentToken = beforeCheck;
                    //Check if an else statement exists...
                    auto elseToken = expectTokenType(TOKEN_TYPE_ELSE);
                    if (elseToken.has_value()) {
                        parseElseStatement(comp);
                    } else {
                        //No else statement, we are done...
                        currentToken = beforeCheck;
                    }
                    break;
                }
            }
        }
    }

    void Parser::parseElifStatement(llvm::Value*& previousStatementSucceeded) {
        Function *function = &Function::functionMap[currentFuncName];
        auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            auto extraProcessing = [](std::vector<Token> &tokens) {
                tokens.pop_back();
            };
            //Check if the previous statement is untrue.
            auto statementValue = expectExpression(nullptr, "{", extraProcessing);
            auto statement = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "elif-statement",
                                                      function->llvmFunction);
            auto afterStatement =
                    llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "after-elif-statement",
                                             function->llvmFunction);
            auto localCondition = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(statementValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));
            auto againstPreviousComp =  LLVMManager::llvmBytecodeBuilder->CreateICmpNE(previousStatementSucceeded, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));
            auto bothConditions = LLVMManager::llvmBytecodeBuilder->CreateAnd(localCondition, againstPreviousComp);
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(bothConditions, statement, afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(statement);
            currentToken--;
            parseFunctionBody();
            LLVMManager::llvmBytecodeBuilder->CreateBr(afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterStatement);
            previousStatementSucceeded = bothConditions;
        }
    }

    void Parser::parseElseStatement(llvm::Value*& previousStatementSucceeded) {
        Function *function = &Function::functionMap[currentFuncName];
        auto statement = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "else-statement",
                                                  function->llvmFunction);
        auto afterStatement =
                llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "after-else-statement",
                                         function->llvmFunction);
        //Check if all previous statements were false.
        auto againstPreviousComp =
                LLVMManager::llvmBytecodeBuilder->CreateICmpNE(previousStatementSucceeded,
                                                               llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_BOOL),
                                                                                      llvm::APInt(1, 1)));
        LLVMManager::llvmBytecodeBuilder->CreateCondBr(againstPreviousComp, statement, afterStatement);
        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(statement);
        parseFunctionBody();
        LLVMManager::llvmBytecodeBuilder->CreateBr(afterStatement);
        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterStatement);
    }
}