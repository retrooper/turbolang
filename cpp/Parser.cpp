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
                    auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR);
                    if (openingParenthesis.has_value()) {
                        bool expectingComma = false;
                        std::vector<llvm::Type *> functionArgumentTypes;
                        bool argumentSizeConstant = true;
                        auto closingParenthesis = expectToken();
                        if (closingParenthesis.has_value() && closingParenthesis.value().text != ")") {
                            currentToken--;
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
                                            "Please separate function declaration arguments with commas! Do NOT specify a name for the types in the arguments.");
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
                                    throw std::runtime_error("Invalid function argument type!");
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

        Token returnToken = *currentToken;
        currentToken++;
        return returnToken;
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

    std::optional<bool> Parser::expectBooleanValue() {
        std::optional<Token> identifier = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            if (identifier.value().text == "true") {
                return true;
            } else if (identifier.value().text == "false") {
                return false;
            }
        }
        return std::nullopt;
    }

    bool Parser::expectFunctionDefinition() {
        std::vector<Token>::iterator parseStart = currentToken;
        auto functionType = expectTokenFunctionType();
        if (functionType.has_value()) {
            auto functionName = expectTokenType(TOKEN_TYPE_IDENTIFIER);
            if (functionName.has_value()) {
                auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
                if (openingParenthesis.has_value()) {
                    Function function(functionName.value().text, functionType.value());
                    while (!expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                        auto parameterType = expectToken();

                        if (!parameterType.has_value()) {
                            throw std::runtime_error("Expected a type at the start of the parameter list!");
                        }
                        FunctionArgument functionArgument;
                        auto varType = Type::getType(parameterType.value().text);
                        if (varType.has_value()) {
                            functionArgument.type = varType.value();
                        }
                        auto variableName = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                        if (variableName.has_value()) {
                            functionArgument.name = variableName.value().text;
                        }
                        function.arguments.push_back(functionArgument);

                        if (expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                            break;
                        }

                        if (!expectTokenType(TOKEN_TYPE_OPERATOR, ",").has_value()) {
                            throw std::runtime_error(
                                    "Expected ',' to separate parameters or ')' to end the parameter list!");
                        }
                    }
                    function.create();

                    Function::functionMap[function.name] = function;
                    currentFuncName = function.name;

                    std::optional<std::vector<Statement>> statements = parseFunctionBody();
                    if (function.type == DATA_TYPE_VOID) {
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

    std::optional<std::vector<Statement>> Parser::parseFunctionBody() {
        if (!expectTokenType(TOKEN_TYPE_OPERATOR, "{").has_value()) {
            return std::nullopt;
        }

        std::vector<Statement> statements;
        std::optional<Token> tempToken;
        while (true) {
            tempToken = expectToken();
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

        if (!expectTokenType(TOKEN_TYPE_OPERATOR, "}").has_value()) {
            throw std::runtime_error("Unbalanced '}'. Line: " + std::to_string(currentToken->lineNumber));
        }
        return statements;
    }

    std::optional<Statement> Parser::parseStatement() {
        std::vector<Token>::iterator startToken;
        struct Statement statement;

        auto typeToken = expectTokenType(
                TOKEN_TYPE_IDENTIFIER);//COULD BE VARIABLE DECLARATION OR VARIABLE MODIFICATION OR FUNCTION CALL
        if (typeToken.has_value()) {
            if (Type::getType(
                    typeToken.value().text).has_value()) { //TODO support custom classes as types, support strings
                //IT IS A VARIABLE DECLARATION
                parseVariableDeclaration(statement, typeToken.value());
                return statement;
            } else {
                //IDENTIFIER
                if (typeToken.value().text == "while") {
                    parseWhileLoop(statement);
                    return statement;
                } else if (typeToken.value().text == "return") {
                    parseReturn(statement);
                    return statement;
                } else {
                    auto equalsOperator = expectTokenType(TOKEN_TYPE_OPERATOR, "=");
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

    void Parser::parseVariableDeclaration(Statement &statement, const Token &typeToken) {
        bool isArray = false;
        int arraySize;
        auto varType = Type::getType(typeToken.text);
        if (varType.has_value()) {
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
                }
                else {
                    varName = variableNameOrOpeningBracketToken.value().text;
                }
                statement.type = VARIABLE_DECLARATION;
                statement.name = varName;
                auto equalsOperator = expectTokenType(TOKEN_TYPE_OPERATOR, "=");
                auto variableValueToken = equalsOperator.has_value() ? expectToken() : std::nullopt;
                llvm::Type *type = Type::getLLVMType(varType.value());
                bool signedType = typeToken.text.find('u') != 0;
                bool checkedForSemiColon = false;
                llvm::Value *allocatedValue = nullptr;

                currentToken--;
                std::optional<Token> nextToken = expectToken();
                while (true) {
                    if (nextToken.has_value()) {
                        if (isMathematicalOperator(nextToken.value().text)) {
                            currentToken--;
                            currentToken--;
                            allocatedValue = parseMathematicalExpression();
                            currentToken--;
                        }
                        else if (nextToken.value().text == "(") {
                            //Skip number after '('
                            currentToken++;
                            nextToken = expectToken();
                            currentToken--;
                            continue;
                        }
                        else if (nextToken.value().type == TOKEN_TYPE_INTEGER_LITERAL ||
                        nextToken.value().type == TOKEN_TYPE_DOUBLE_LITERAL) {
                            nextToken = expectToken();
                            continue;
                        }
                        break;
                    }
                }
                if (allocatedValue == nullptr && variableValueToken.has_value()) {
                    switch (variableValueToken.value().type) {
                        case TOKEN_TYPE_IDENTIFIER:
                            if (expectTokenType(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                                //ITS A FUNCTION CALL!
                                currentToken--;
                                allocatedValue = parseFunctionCall(statement, variableValueToken.value().text);
                                checkedForSemiColon = true;
                            } else {
                                if (variableValueToken.value().text.find('&') == 0) {
                                    //Starts with '&', its a pointer
                                    allocatedValue = Function::functionMap[currentFuncName].getAllocaInst(
                                            variableValueToken.value().text.substr(1));

                                } else {
                                    currentToken--;
                                    auto boolVal = expectBooleanValue();
                                    if (boolVal.has_value()) {
                                        allocatedValue = llvm::ConstantInt::get(type, llvm::APInt(1, boolVal.value() ? 1
                                                                                                                     : 0));
                                    } else {
                                        allocatedValue = Function::functionMap[currentFuncName].getValue(
                                                variableValueToken.value().text);
                                    }
                                }
                            }
                            break;
                        case TOKEN_TYPE_INTEGER_LITERAL:
                            allocatedValue = llvm::ConstantInt::get(type, std::stol(variableValueToken.value().text),
                                                                    signedType);
                            break;
                        case TOKEN_TYPE_DOUBLE_LITERAL:
                            allocatedValue = llvm::ConstantFP::get(type,
                                                                   std::stod(variableValueToken.value().text));
                            break;
                        case TOKEN_TYPE_STRING_LITERAL:
                            allocatedValue = LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(
                                    variableValueToken.value().text, varName);
                            break;
                        default:
                            throw std::runtime_error("Unsupported variable value Token!");
                    }
                }
                llvm::AllocaInst* allocaInst;
                if (isArray) {
                    allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type,
                                                                                llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_INT),
                                                                                                       llvm::APInt(32, arraySize)),
                                                                                                       llvm::Twine(varName));
                }
                else {
                    allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type, nullptr,
                                                                                                  llvm::Twine(
                                                                                                          varName));
                }
                Function::functionMap[currentFuncName].setAllocaInst(
                        varName, allocaInst);
                if (variableValueToken.has_value()) {
                    if (allocatedValue == nullptr) {
                        allocatedValue = allocaInst->getOperand(0);
                    }

                    Function::functionMap[currentFuncName].setValue(varName, allocatedValue);
                }

                if (!checkedForSemiColon && !expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                    throw std::runtime_error("Expected a semicolon!");
                }
            }
        }
    }


    void Parser::parseVariableModification(Statement &statement,
                                           const std::optional<Token> &variableNameToken) {
        bool checkedForSemiColon = false;
        std::optional<Token> variableValueToken = expectToken();
        if (variableValueToken.has_value()) {
            statement.type = VARIABLE_MODIFICATION;
            statement.name = variableNameToken.value().text;
            llvm::Value *val = nullptr;
            currentToken--;
            std::optional<Token> nextToken = expectToken();
            while (true) {
                if (nextToken.has_value()) {
                    if (isMathematicalOperator(nextToken.value().text)) {
                        currentToken--;
                        currentToken--;
                        val = parseMathematicalExpression();
                        currentToken--;
                    }
                    else if (nextToken.value().text == "(") {
                        //Skip number after '('
                        currentToken++;
                        nextToken = expectToken();
                        currentToken--;
                        continue;
                    }
                    else if (nextToken.value().type == TOKEN_TYPE_INTEGER_LITERAL ||
                             nextToken.value().type == TOKEN_TYPE_DOUBLE_LITERAL) {
                        nextToken = expectToken();
                        continue;
                    }
                    break;
                }
            }
            if (val == nullptr) {
                switch (variableValueToken.value().type) {
                    case TOKEN_TYPE_IDENTIFIER:
                        if (expectTokenType(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                            //ITS A FUNCTION CALL!
                            currentToken--;
                            val = parseFunctionCall(statement, variableValueToken.value().text);
                            checkedForSemiColon = true;
                        } else {
                            if (variableValueToken.value().text.find('&') == 0) {
                                //Starts with '&', its a pointer
                                val = Function::functionMap[currentFuncName].getAllocaInst(
                                        variableValueToken.value().text.substr(1));
                            } else {
                                currentToken--;
                                auto boolVal = expectBooleanValue();
                                if (boolVal.has_value()) {
                                    val = llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_BOOL),
                                                                 llvm::APInt(1, boolVal.value() ? 1 : 0));
                                } else {
                                    val = Function::functionMap[currentFuncName].getValue(
                                            variableValueToken.value().text);
                                }
                            }
                        }
                        break;
                    case TOKEN_TYPE_INTEGER_LITERAL:
                        //TODO get the initial variable type, and make to set isSigned to correct value and its type correctly
                        val = llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_INT),
                                                     std::stoi(variableValueToken.value().text), true);
                        break;
                    case TOKEN_TYPE_DOUBLE_LITERAL:
                        //TODO get initial var type
                        val = llvm::ConstantFP::get(Type::getLLVMType(DATA_TYPE_DOUBLE),
                                                    std::stod(variableValueToken.value().text));
                        break;
                    case TOKEN_TYPE_STRING_LITERAL:
                        val = LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(
                                variableValueToken.value().text);
                        break;
                    default:
                        throw std::runtime_error("Unsupported variable value Token!");
                }

            }
            Function::functionMap[currentFuncName].setValue(variableNameToken.value().text, val);
            if (!checkedForSemiColon && !expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    llvm::Value *Parser::parseFunctionCall(Statement &statement,
                                           const std::string &functionName) {
        auto openingParenthesisOperator = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesisOperator.has_value()) {
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

            if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
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
                            llvmFunctionArgument = Function::functionMap[currentFuncName].getValue(argument.text);
                        }
                        break;
                    case TOKEN_TYPE_INTEGER_LITERAL:
                        //TODO create byte, short, long
                        llvmFunctionArgument = llvm::ConstantInt::get(*LLVMManager::llvmCtx,
                                                                      llvm::APInt(32, std::stoi(argument.text)));
                        break;
                    case TOKEN_TYPE_DOUBLE_LITERAL:
                        //TODO support explicit float literals
                        llvmFunctionArgument = llvm::ConstantFP::get(Type::getLLVMType(
                                DATA_TYPE_DOUBLE), //TODO get type the function takes in its args
                                                                     std::stod(argument.text));
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
        return nullptr;
    }

    void Parser::parseWhileLoop(Statement &statement) {
        auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            llvm::Value *boolValue;
            auto literalBoolValue = expectBooleanValue(); //boolean literal
            std::optional<Token> identifier = std::nullopt;
            if (literalBoolValue.has_value()) {
                boolValue = llvm::ConstantInt::get(Type::getLLVMType(DATA_TYPE_BOOL),
                                                   llvm::APInt(1, literalBoolValue.value() ? 1 : 0));
            } else {
                //TODO handle variable
                //TODO handle math expressions
                currentToken--;
                identifier = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                if (identifier.has_value()) {
                    boolValue = Function::functionMap[currentFuncName].getValue(identifier.value().text);
                }
            }
            auto closingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, ")");
            if (closingParenthesis.has_value()) {
                Function *function = &Function::functionMap[currentFuncName];
                llvm::BasicBlock *loop = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "loop",
                                                                  function->llvmFunction);

                llvm::BasicBlock *afterLoop =
                        llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "afterloop",
                                                 function->llvmFunction);

                auto comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(boolValue, llvm::ConstantInt::get(
                        Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));
                LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, loop, afterLoop);
                LLVMManager::llvmBytecodeBuilder->SetInsertPoint(loop);

                parseFunctionBody();
                if (identifier.has_value()) {
                    boolValue = Function::functionMap[currentFuncName].getValue(identifier.value().text);
                    comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(boolValue, llvm::ConstantInt::get(
                            Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)));
                }

                LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, loop, afterLoop);
                LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterLoop);
            }
        }
    }

    void Parser::parseReturn(Statement &statement) {
        auto returnValue = expectToken();
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
                    if (expectTokenType(TOKEN_TYPE_OPERATOR, "(").has_value()) {
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
        if (!checkedForSemiColon && !expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
            throw std::runtime_error("Expected semicolon!");
        }
    }

    llvm::Value *eval(llvm::Value *a, llvm::Value *b, std::string op) {
        if (op == "+") {
            return LLVMManager::llvmBytecodeBuilder->CreateAdd(a, b);
        } else if (op == "-") {
            return LLVMManager::llvmBytecodeBuilder->CreateSub(a, b);
        } else if (op == "*") {
            return nullptr;  //TODO
        } else if (op == "/") {
            return nullptr; //TODO
        } else {
            throw std::runtime_error("Unsupported operator");
        }
    }

    llvm::Value *Parser::parseMathematicalExpression() {
        std::optional<Token> nextToken;
        std::vector<Token> expressionTokens;
        while (true) {
            nextToken = expectToken();
            if (nextToken.has_value() && nextToken.value().text != ";") {
                expressionTokens.push_back(nextToken.value());
            } else {
                break;
            }
        }
        return MathEvaluator::eval(expressionTokens);
    }

    bool Parser::isMathematicalOperator(const std::string &op) {
        return op == "+" || op == "-" || op == "*" || op == "/";
    }
}