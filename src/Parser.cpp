#include "parser/Parser.h"

namespace turbolang {
    std::vector<Token>::iterator Parser::currentToken;
    std::vector<Token>::iterator Parser::endToken;
    Function *Parser::currentFunction = nullptr;
    std::vector<std::string> Parser::modules;

    void Parser::parse(std::vector<Token> &tokens) {
        endToken = tokens.end();
        currentToken = tokens.begin();
        if (currentToken->type == TOKEN_TYPE_IMPORT) {
            parseImportStatement();
        }
        //MAIN FUNCTION
        while (currentToken != endToken) {
            if (currentToken->type == TOKEN_TYPE_DECLARE) {
                currentToken++;
                parseFunctionDeclareStatement();
                continue;
            } else if (currentToken->type == TOKEN_TYPE_LINK) {
                currentToken++;
                parseLinkStatement();
                continue;
            } else if (currentToken->type == TOKEN_TYPE_CLASS) {
                currentToken++;
                parseClassDefinition();
                continue;
            }
            if (expectFunctionDefinition()) {

            } else {
                std::cerr << "Unknown identifier " << currentToken->text << std::endl;
                currentToken++;
            }
        }
    }

    void Parser::parseClassDefinition() {
        //Check for the class name
        auto nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (nextToken.has_value()) {
            Class clazz(nextToken.value().text);
            clazz.create();
            std::vector<llvm::Type *> members;
            //Check for the class scope entry
            auto scopeEntry = expectTokenType(TOKEN_TYPE_OPERATOR, "{");
            if (scopeEntry.has_value()) {
                while (!expectTokenType(TOKEN_TYPE_OPERATOR, "}").has_value()) {
                    //Variable declaration
                    auto dataTypeIdentifier = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                    if (dataTypeIdentifier.has_value()) {
                        uint32_t pointerCount = 0;
                        while (true) {
                            auto potentialNextToken = expectTokenType(TOKEN_TYPE_OPERATOR, "*");
                            if (potentialNextToken.has_value()) {
                                pointerCount++;
                            } else {
                                break;
                            }
                        }
                        auto dataType = Type::getType(dataTypeIdentifier.value().text);
                        auto variableName = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                        if (variableName.has_value()) {
                            if (expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                                ClassMemberData clsMemberData;
                                clsMemberData.name = variableName.value().text;
                                clsMemberData.type = Type::getLLVMType(dataType.value(),
                                                                       dataTypeIdentifier.value().text);
                                clsMemberData.isSigned = Type::isTypeSigned(dataType.value());
                                llvm::Type *type = clsMemberData.type;
                                for (uint32_t i = 0; i < pointerCount; i++) {
                                    type = type->getPointerTo();
                                }
                                clsMemberData.type = type;
                                clazz.clsMemberData.push_back(clsMemberData);
                                members.push_back(clsMemberData.type);
                            } else {
                                throw std::runtime_error(
                                        &"Expected semicolon after variable declaration inside a class. Line: "[currentToken->lineNumber]);
                            }
                        }
                    } else {
                        currentToken--;
                        throw std::runtime_error("Unexpected token " + expectToken().value().text +
                                                 " in the definition of the class " + clazz.name);
                    }
                }

                clazz.structType->setBody(members);
                Class::classMap[clazz.name] = clazz;
            } else {
                throw std::runtime_error("Could not find " + clazz.name + "'s class scope!");
            }
        } else {
            throw std::runtime_error("Invalid class name: " + nextToken.value().text);
        }
    }

    void Parser::parseFunctionDeclareStatement() {
        auto nextToken = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (nextToken.has_value()) {
            bool isPointer = false;
            auto potentialNextToken = expectTokenType(TOKEN_TYPE_OPERATOR, "*");
            if (potentialNextToken.has_value()) {
                isPointer = true;
            }
            auto functionTypeOptional = Type::getType(nextToken.value().text);
            std::string functionTypeStr = nextToken.value().text;
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
                                        uint32_t pointerCount = 0;
                                        while (true) {
                                            potentialNextToken = expectTokenType(TOKEN_TYPE_OPERATOR, "*");
                                            if (potentialNextToken.has_value()) {
                                                pointerCount++;
                                            } else {
                                                break;
                                            }
                                        }
                                        functionTypeOptional = Type::getType(nextToken.value().text);
                                        if (functionTypeOptional.has_value()) {
                                            auto functionArgumentType = functionTypeOptional.value();
                                            llvm::Type *type = Type::getLLVMType(functionArgumentType,
                                                                                 nextToken.value().text);
                                            for (uint32_t i = 0; i < pointerCount; i++) {
                                                type = type->getPointerTo();
                                            }
                                            functionArgumentTypes.push_back(type);
                                            expectingComma = true;
                                            continue;
                                        } else {
                                            //TODO
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
                            llvm::Type *returnType = Type::getLLVMType(functionType, functionTypeStr);
                            if (isPointer) {
                                returnType = returnType->getPointerTo();
                            }

                            FunctionInternal::defineFunction(functionName,
                                                             returnType,
                                                             functionArgumentTypes, argumentSizeConstant);
                        } else {
                            throw std::runtime_error("Expected semicolon!");
                        }
                    }
                }
            }
        }
    }

    void Parser::parseLinkStatement() {
        auto libraryToken = expectTokenType(TOKEN_TYPE_STRING_LITERAL);
        if (libraryToken.has_value()) {
            std::string libraryName = libraryToken.value().text;
            Builder::libraries.push_back(libraryName);
        } else {
            std::cerr << "Expected a library name to link against! For example: link \"pthread\"" << std::endl;
            std::exit(-1);
        }
    }

    void Parser::parseModule(std::string &module) {
        std::string extension = "tl";
        int dotIndex = module.find('.');
        if (dotIndex == std::string::npos || module.substr(dotIndex) != extension) {
            module += "." + extension;
        }
        //We only parse new modules. We will not parse the same module twice.
        if (!(std::find(modules.begin(), modules.end(), module) != modules.end())) {
            auto basePath = std::filesystem::current_path();
            auto modulesPath = basePath.string() + "/modules";
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
            auto prevFunc = currentFunction;
            Parser::parse(tokens);
            currentToken = prevCurrToken;
            endToken = prevEndToken;
            currentFunction = prevFunc;
            modules.push_back(module);
        }
    }

    void Parser::parseImportStatement() {
        bool importPending = false;
        while (true) {
            auto nextToken = expectTokenType(TOKEN_TYPE_IMPORT);
            if (nextToken.has_value()) {
                importPending = true;
            } else {
                if (importPending) {
                    nextToken = expectTokenType(TOKEN_TYPE_STRING_LITERAL);
                    if (nextToken.has_value()) {
                        std::string module = nextToken.value().text;
                        parseModule(module);
                        importPending = false;
                    } else {
                        throw std::runtime_error("Invalid module format!");
                    }
                } else {
                    break;
                }
            }
        }
    }

    llvm::Value *Parser::expectExpression(const DataType &resultType, const std::string &className, const Token *token,
                                          const std::string &endAtStr,
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

        if (expressionTokens.size() == 3 && expressionTokens[1].text == ".") {
            return currentFunction->getValue(
                    expressionTokens[0].text + "." + expressionTokens[2].text);
        }

        return MathEvaluator::eval(expressionTokens, *currentFunction, resultType, className);
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

    bool Parser::expectFunctionDefinition() {
        auto parseStart = currentToken;
        //Function return type
        std::optional<Token> returnTypeIdentifier = expectTokenType(TOKEN_TYPE_IDENTIFIER);
        if (returnTypeIdentifier.has_value()) {
            std::optional<DataType> optionalFuncType = Type::getType(returnTypeIdentifier.value().text);
            if (optionalFuncType.has_value()) {
                auto returnType = optionalFuncType.value();
                //Function name
                auto functionName = expectTokenType(TOKEN_TYPE_IDENTIFIER);
                if (functionName.has_value()) {
                    auto prevCurToken = currentToken;
                    bool isAFunction = expectTokenType(TOKEN_TYPE_OPERATOR, "(").has_value() ||
                                       expectTokenType(TOKEN_TYPE_OPERATOR, "()").has_value();
                    if (!isAFunction) {
                        currentToken = parseStart;
                        return false;
                    }
                    currentToken = prevCurToken;
                    //Opening parenthesis for the function arguments
                    bool hasArguments = !expectTokenType(TOKEN_TYPE_OPERATOR, "()").has_value();
                    if (hasArguments) {
                        currentToken++;
                    }
                    Function function(functionName.value().text, returnType,
                                      returnType == DATA_TYPE_CLASS ? returnTypeIdentifier.value().text : "");
                    while (hasArguments && !expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
                        auto parameterType = expectToken();
                        if (!parameterType.has_value()) {
                            throw std::runtime_error("Expected a type at the start of the parameter list!");
                        }
                        auto potentialNextToken = expectTokenType(TOKEN_TYPE_OPERATOR, "*");
                        bool isPointer = false;
                        if (potentialNextToken.has_value()) {
                            isPointer = true;
                        }
                        //Function parameter
                        FunctionArgument functionArgument;
                        functionArgument.isPtr = isPointer;
                        //Parameter type
                        auto varType = Type::getType(parameterType.value().text);
                        if (varType.has_value()) {
                            functionArgument.type = varType.value();
                            functionArgument.typeInfo = parameterType.value().text;
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
                    Function::functions.push_back(function);
                    currentFunction = &Function::functions[Function::functions.size() - 1];
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
                }
            }
        }

        currentToken = parseStart;
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
                case TOKEN_TYPE_OPERATOR:
                    //Is it a valid data type?
                    if (Type::getType(
                            token.value().text).has_value()) { //TODO support custom classes as types, support strings
                        //Then a variable is for sure being declared/created.
                        auto potentialNextToken = expectTokenType(TOKEN_TYPE_OPERATOR, "*");
                        bool isPointer = false;
                        if (potentialNextToken.has_value()) {
                            isPointer = true;
                        }
                        parseVariableDeclaration(Type::getType(token.value().text), isPointer,
                                                 token.value().text);
                    } else {
                        bool isDereferencing = false;
                        if (token.value().text == "*") {
                            //Dereference
                            isDereferencing = true;
                            token = expectToken();
                        }
                        //Is the next token an operator?
                        auto operatorToken = expectTokenType(TOKEN_TYPE_OPERATOR);
                        if (operatorToken.has_value()) {
                            //Is it an assignment? If yes we are modifying a variable.
                            if (operatorToken.value().text == "=") {
                                parseVariableModification(token, isDereferencing);
                            }
                                //Is it an opening parenthesis? If yes we are calling a function.
                            else if (operatorToken.value().text == "(") {
                                parseFunctionCall(token.value().text);
                                if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                                    throw std::runtime_error(
                                            &"Expected a semicolon at line "[currentToken->lineNumber]);
                                }
                            } else if (operatorToken.value().text == ".") {
                                std::string name = token.value().text + ".";
                                while (true) {
                                    auto nextToken = expectToken();
                                    if (nextToken.has_value() && nextToken.value().type == TOKEN_TYPE_OPERATOR &&
                                        nextToken.value().text == "=") {
                                        break;
                                    }
                                    name += nextToken.value().text;
                                }
                                Token tkn{};
                                tkn.lineNumber = currentToken->lineNumber;
                                tkn.text = name;
                                tkn.type = TOKEN_TYPE_IDENTIFIER;
                                parseVariableModification(tkn, isDereferencing);
                            } else {
                                throw std::runtime_error(
                                        "Unexpected operator token. Expected an assignment operator or an opening parenthesis! Line: " +
                                        std::to_string(currentToken->lineNumber));
                            }
                        } else {
                            throw std::runtime_error(&"Unexpected token. Line: "[currentToken->lineNumber]);
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
                    "Failed to parse a statement. No token found? Line: " +
                    std::to_string(currentToken
                                           ->lineNumber));
        }
    }

    void Parser::parseVariableDeclaration(const std::optional<DataType> &varType, bool isPointer,
                                          const std::string &className) {
        //Is the declared variable an array.
        bool isArray = false;
        //If the variable is an array, we will store its size in this variable.
        int arraySize;
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

                llvm::Type *type = Type::getLLVMType(varType.value(), className);
                if (isArray) {
                    type = llvm::ArrayType::get(type, arraySize);
                }
                if (isPointer) {
                    type = type->getPointerTo();
                }
                llvm::AllocaInst *allocaInst;
                allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type,
                                                                            nullptr,
                                                                            llvm::Twine(varName));
                currentFunction->setAllocaInst(
                        varName, allocaInst);

                        std::cout << "pre check equals, var name: " << varName << std::endl;
                        llvm::outs() << ", alloca: " << *allocaInst->getType() << "\n";


                auto equalsOperator = expectTokenType(TOKEN_TYPE_OPERATOR, "=");
                //bool signedType = typeToken.text.find('u') != 0;
                llvm::Value *allocatedValue = nullptr;
                //Check if we are also allocating and not just declaring the variable.
                if (equalsOperator.has_value()) {
                    //Handle an expression.
                    allocatedValue = expectExpression(varType.value(), className);

                    if (allocatedValue == nullptr) {
                        allocatedValue = allocaInst->getOperand(0);
                    }

                    if (type->isPointerTy()) {
                        if (allocatedValue->getType()->getPointerElementType()->isArrayTy()) {
                            llvm::outs() << "output: " << *allocatedValue->getType() << "\n";
                            allocatedValue = LLVMManager::llvmBytecodeBuilder->CreateBitCast(allocatedValue, type, "VariableDeclarationBitCast");
                        }
                    }

                    currentFunction->setValue(varName, allocatedValue);
                }
                else {
                    //TODO We were here!
                    allocatedValue = llvm::ConstantPointerNull::get(llvm::PointerType::get(
                                Type::getLLVMType(DATA_TYPE_CLASS,
                                                  "File"), 0));
                    currentFunction->setValue(varName, allocatedValue);
                }

                if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value() && className.empty()) {
                    throw std::runtime_error("Expected a semicolon!");
                }
            }
        }
    }


    void Parser::parseVariableModification(const std::optional<Token> &variableNameToken, bool isDereferencing) {
        llvm::Type *allocaType = currentFunction->getType(variableNameToken.value().text);
        //TODO check if the original variable is signed
        auto dataType = Type::getType(allocaType, true);
        if (dataType.has_value()) {
            //Handle an expression
            std::string className = dataType == DATA_TYPE_CLASS ? allocaType->isPointerTy()
                                                                  ? (allocaType->getPointerElementType()->getStructName().str())
                                                                  : (allocaType->getStructName().str()) : "";
            llvm::Value *val = expectExpression(dataType.value(), className);
            if (isDereferencing) {
                currentFunction->setDereferencedValue(variableNameToken.value().text, val);
            } else {
                currentFunction->setValue(variableNameToken.value().text, val);
            }
            if (!expectTokenType(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        } else {
            std::cerr << "Failed to access type of variable you are trying to modify. Line: "
                      << currentToken->lineNumber
                      << std::endl;
            std::exit(-1);
        }
    }

    llvm::Value *Parser::parseFunctionCall(const std::string &functionName) {
        std::vector<std::vector<Token>> arguments;
        std::vector<Token> tokens;
        while (!expectTokenType(TOKEN_TYPE_OPERATOR, ")").has_value()) {
            while (true) {
                auto newToken = expectToken();
                if (!newToken.has_value() || (newToken.value().type == TOKEN_TYPE_OPERATOR &&
                                              (newToken.value().text == "," || newToken.value().text == ")"))) {
                    currentToken--;
                    break;
                }
                auto parameter = newToken.value();
                tokens.push_back(parameter);
            }
            auto op = expectTokenType(TOKEN_TYPE_OPERATOR);
            if (op.has_value()) {
                arguments.push_back(tokens);
                tokens.clear();
                if (op.value().text == ")") {
                    break;
                }
            }
        }
        std::vector<llvm::Value *> llvmFunctionArguments;
        llvm::Function *llvmFunction = LLVMManager::llvmModule->getFunction(functionName);
        if (llvmFunction == nullptr) {
            std::cerr << "Failed to find any function named " << functionName << "!\n";
            std::exit(-1);
        }
        if (llvmFunction->arg_size() != arguments.size() && !llvmFunction->isVarArg()) {
            std::cerr << "Failed to call function " << functionName << ", the function is defined with "
                      << llvmFunction->arg_size() << " arguments, but you are expecting " << arguments.size()
                      << " arguments!" << std::endl;
            std::exit(-1);
        }
        for (int i = 0; i < arguments.size(); i++) {
            DataType resultType = DATA_TYPE_UNKNOWN;
            std::string resultClassName;
            auto argument = arguments[i];
            llvm::Type *castedType = nullptr;
            if (!llvmFunction->isVarArg() /*&& llvmFunction->arg_size() <= i + 1*/) {
                auto arg = llvmFunction->getArg(i);
                auto dataTypeOptional = Type::getType(arg->getType(), true);
                resultType = dataTypeOptional.has_value() ? dataTypeOptional.value() : DATA_TYPE_UNKNOWN;
                if (resultType == DATA_TYPE_CLASS) {
                    resultClassName = arg->getType()->isPointerTy()
                                      ? arg->getType()->getPointerElementType()->getStructName().str()
                                      : arg->getType()->getStructName().str();
                }
                if (arg->getType()->isPointerTy()) {
                    castedType = arg->getType();
                }
            }
            //TODO support functions returning custom types
            llvm::Value *llvmFunctionArgument = MathEvaluator::eval(argument, *currentFunction, resultType,
                                                                    resultClassName);
            if (castedType != nullptr && castedType != llvmFunctionArgument->getType()) {
                llvm::outs() << "LLVM PREV TYPE: " << *llvmFunctionArgument->getType() << "\n";
                llvmFunctionArgument = LLVMManager::llvmBytecodeBuilder->CreateBitCast(llvmFunctionArgument, castedType,
                                                                                       "PtrBitCast");
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
            llvm::Value *whileLoopValue = expectExpression(DATA_TYPE_BOOL, "", nullptr, "{", extraProcessing);
            llvm::BasicBlock *loop = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "loop",
                                                              currentFunction->llvmFunction);

            llvm::BasicBlock *afterLoop =
                    llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "afterloop",
                                             currentFunction->llvmFunction);

            auto comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(whileLoopValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)), "EqualComparisonWhileLoopFirst");
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, loop, afterLoop);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(loop);
            currentToken--;
            parseFunctionBody();
            auto prevCurrentTokenState = currentToken;
            currentToken = loopCheckState;
            whileLoopValue = expectExpression(DATA_TYPE_BOOL, "", nullptr, "{", extraProcessing);
            currentToken = prevCurrentTokenState;
            comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(whileLoopValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)), "EqualComparisonWhileLoopSecond");

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
                llvmReturnValue = expectExpression(currentFunction->type, "", &returnValue.value());
            }
        } else {
            std::cerr << "Invalid return statement! Line: " << currentToken->lineNumber << std::endl;
            std::exit(-1);
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
            llvm::Value *statementValue = expectExpression(DATA_TYPE_BOOL, "", nullptr, "{", extraProcessing);
            llvm::BasicBlock *statement = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "if-statement",
                                                                   currentFunction->llvmFunction);

            llvm::BasicBlock *afterStatement =
                    llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "after-if-statement",
                                             currentFunction->llvmFunction);

            llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "statement-end-block",
                                                                  currentFunction->llvmFunction);


            auto comp = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(statementValue, llvm::ConstantInt::get(
                    Type::getLLVMType(DATA_TYPE_BOOL), llvm::APInt(1, 1)), "EqualComparisonIfStatement");
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, statement, afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(statement);
            currentToken--;
            parseFunctionBody();
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(comp, endBlock, afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterStatement);
            auto beforeCheck = currentToken;
            while (true) {
                auto elseIfToken = expectTokenType(TOKEN_TYPE_ELSE_IF);
                if (elseIfToken.has_value()) {
                    parseElifStatement(endBlock);
                    beforeCheck = currentToken;
                    //Check if an else statement exists...
                    auto elseToken = expectTokenType(TOKEN_TYPE_ELSE);
                    if (elseToken.has_value()) {
                        parseElseStatement(endBlock);
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
                        parseElseStatement(endBlock);
                    } else {
                        //No else statement, we are done...
                        currentToken = beforeCheck;
                    }
                    break;
                }
            }
            LLVMManager::llvmBytecodeBuilder->CreateBr(endBlock);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(endBlock);
        }
    }

    void Parser::parseElifStatement(llvm::BasicBlock *&endBlock) {
        auto openingParenthesis = expectTokenType(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesis.has_value()) {
            auto extraProcessing = [](std::vector<Token> &tokens) {
                tokens.pop_back();
            };
            auto statementValue = expectExpression(DATA_TYPE_BOOL, "", nullptr, "{", extraProcessing);
            auto statement = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "elif-statement",
                                                      currentFunction->llvmFunction);
            auto afterStatement =
                    llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "after-elif-statement",
                                             currentFunction->llvmFunction);
            auto localCondition = LLVMManager::llvmBytecodeBuilder->CreateICmpEQ(statementValue,
                                                                                 llvm::ConstantInt::get(
                                                                                         Type::getLLVMType(
                                                                                                 DATA_TYPE_BOOL),
                                                                                         llvm::APInt(1, 1)));
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(localCondition, statement, afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(statement);
            currentToken--;
            parseFunctionBody();
            LLVMManager::llvmBytecodeBuilder->CreateCondBr(localCondition, endBlock, afterStatement);
            LLVMManager::llvmBytecodeBuilder->SetInsertPoint(afterStatement);
        }
    }

    void Parser::parseElseStatement(llvm::BasicBlock *&endBlock) {
        auto statement = llvm::BasicBlock::Create(*LLVMManager::llvmCtx, "else-statement",
                                                  currentFunction->llvmFunction);
        LLVMManager::llvmBytecodeBuilder->CreateBr(statement);
        LLVMManager::llvmBytecodeBuilder->SetInsertPoint(statement);
        parseFunctionBody();
        LLVMManager::llvmBytecodeBuilder->CreateBr(endBlock);
    }

}