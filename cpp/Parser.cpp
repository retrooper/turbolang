#include "parser/Parser.h"

namespace turbolang {
    std::vector<Token>::iterator Parser::currentToken;
    std::vector<Token>::iterator Parser::endToken;
    std::string Parser::currentFuncName;


    void Parser::load() {
        FunctionCallProcessor::load();
    }

    void Parser::unload() {
        FunctionCallProcessor::unload();
    }

    void Parser::parse(std::vector<Token> &tokens) {
        endToken = tokens.end();
        currentToken = tokens.begin();
        if (currentToken->type == TOKEN_TYPE_IDENTIFIER && currentToken->text == "import") {
            parseImportStatement();
        }
        //MAIN FUNCTION
        while (currentToken != endToken) {
            if (expect_function_definition()) {

            } else {
                std::cerr << "Unknown identifier " << currentToken->text << std::endl;
                currentToken++;
            }
        }
    }

    void Parser::parseImportStatement() {
        std::vector<std::string> importedModules;
        bool importPending = false;
        while (true) {
            auto nextToken = expect_token_type(TOKEN_TYPE_IDENTIFIER, "import");
            if (nextToken.has_value()) {
                importPending = true;
            } else {
                if (importPending) {
                    nextToken = expect_token_type(TOKEN_TYPE_STRING_LITERAL);
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
            std::cout << "code: " << code << std::endl;
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

    std::optional<Token> Parser::expect_token_type(const TokenType &type, const std::string &name) {
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

    std::optional<Token> Parser::expect_token_type(const TokenType &type) {
        return expect_token_type(type, "");
    }

    std::optional<Token> Parser::expect_token() {
        if (currentToken == endToken) {
            return std::nullopt;
        }

        Token returnToken = *currentToken;
        currentToken++;
        return returnToken;
    }

    std::optional<VariableType> Parser::expect_token_variable_type() {
        std::optional<Token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            std::optional<int> optionalVarType = Type::getVariableType(identifier.value().text);
            if (optionalVarType.has_value()) {
                return static_cast<VariableType>(optionalVarType.value());
            }
        }
        return std::nullopt;
    }

    std::optional<FunctionType> Parser::expect_token_function_type() {
        std::optional<Token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (identifier.has_value()) {
            std::optional<int> optionalFuncType = Type::getFunctionType(identifier.value().text);
            if (optionalFuncType.has_value()) {
                return static_cast<FunctionType>(optionalFuncType.value());
            }
        }
        return std::nullopt;
    }

    std::optional<bool> Parser::expect_boolean_value() {
        std::optional<Token> identifier = expect_token_type(TOKEN_TYPE_IDENTIFIER);
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
        std::vector<Token>::iterator parseStart = currentToken;
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
        std::optional<Token> tempToken;
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
            std::cerr << currentToken->text << std::endl;
            throw std::runtime_error("Unbalanced '}'. Line: " + std::to_string(currentToken->lineNumber));
        }
        return statements;
    }

    std::optional<Statement> Parser::parseStatement() {
        std::vector<Token>::iterator startToken;
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

    void Parser::parseVariableDeclaration(Statement &statement, const Token &typeToken) {
        auto varType = Type::getVariableType(typeToken.text);
        if (varType.has_value()) {
            auto variableNameToken = expect_token_type(TOKEN_TYPE_IDENTIFIER);
            if (variableNameToken.has_value()) {
                statement.type = VARIABLE_DECLARATION;
                statement.name = variableNameToken.value().text;
                auto equalsOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "=");
                auto variableValueToken = equalsOperator.has_value() ? expect_token() : std::nullopt;
                llvm::Type *type = Type::getLLVMTypeByVariableType(varType.value());
                bool checkedForSemiColon = false;
                llvm::Value *allocatedValue;
                if (variableValueToken.has_value()) {
                    switch (variableValueToken.value().type) {
                        case TOKEN_TYPE_IDENTIFIER:
                            if (expect_token_type(TOKEN_TYPE_OPERATOR, "(").has_value()) {
                                //ITS A FUNCTION CALL!
                                currentToken--;
                                allocatedValue = parseFunctionCall(statement, variableValueToken.value().text);
                                checkedForSemiColon = true;
                            } else {
                                if (variableValueToken.value().text.find('&') == 0) {
                                    //Starts with '&', its a pointer
                                    allocatedValue = Function::functionMap[currentFuncName].getAllocaInst(
                                            variableValueToken.value().text);
                                } else {
                                    allocatedValue = Function::functionMap[currentFuncName].getValue(
                                            variableValueToken.value().text);
                                }
                            }
                            break;
                        case TOKEN_TYPE_INTEGER_LITERAL:
                            allocatedValue = llvm::ConstantInt::get(type,
                                                                    llvm::APInt(Type::getVariableIntBitCount(
                                                                            typeToken.text), std::stoi(
                                                                            variableValueToken.value().text)));
                            break;
                        case TOKEN_TYPE_DOUBLE_LITERAL:
                            allocatedValue = llvm::ConstantFP::get(type,
                                                                   std::stod(variableValueToken.value().text));
                            break;
                        case TOKEN_TYPE_STRING_LITERAL:
                            allocatedValue = LLVMManager::llvmBytecodeBuilder->CreateGlobalStringPtr(
                                    variableValueToken.value().text, variableNameToken.value().text);
                            break;
                        default:
                            throw std::runtime_error("Unsupported variable value Token!");
                    }
                }
                llvm::AllocaInst *allocaInst = LLVMManager::llvmBytecodeBuilder->CreateAlloca(type, nullptr,
                                                                                              llvm::Twine(
                                                                                                      variableNameToken.value().text));
                Function::functionMap[currentFuncName].setAllocaInst(
                        variableNameToken.value().text, allocaInst);
                if (variableValueToken.has_value()) {
                    if (allocatedValue == nullptr) {
                        allocatedValue = allocaInst->getOperand(0);
                    }

                    Function::functionMap[currentFuncName].setValue(variableNameToken.value().text, allocatedValue);
                }

                if (!checkedForSemiColon && !expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                    throw std::runtime_error("Expected a semicolon!");
                }
            }
        }
    }


    void Parser::parseVariableModification(Statement &statement,
                                           const std::optional<Token> &variableNameToken) {
        std::optional<Token> variableValueToken = expect_token();
        if (variableValueToken.has_value()) {
            statement.type = VARIABLE_MODIFICATION;
            statement.name = variableNameToken.value().text;
            llvm::Type *type;
            llvm::Value *val = nullptr;
            std::optional<Token> nextToken = expect_token();
            currentToken--;
            if (nextToken.has_value() && nextToken.value().type == TOKEN_TYPE_OPERATOR &&
                isMathematicalOperator(nextToken.value().text)) {
                currentToken--;
                val = parseMathematicalExpression();
                currentToken--;
            }
            if (val == nullptr) {
                switch (variableValueToken.value().type) {
                    case TOKEN_TYPE_IDENTIFIER:
                        val = Function::functionMap[currentFuncName].getValue(variableValueToken.value().text);
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
                        throw std::runtime_error("Unsupported variable value Token!");
                }
                Function::functionMap[currentFuncName].setValue(variableNameToken.value().text, val);
            }
            if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    llvm::Value *Parser::parseFunctionCall(Statement &statement,
                                           const std::string &functionName) {
        auto openingParenthesisOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
        if (openingParenthesisOperator.has_value()) {
            std::vector<Token> arguments;
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
                        llvmFunctionArgument = llvm::ConstantFP::get(Type::getLLVMTypeByVariableType(
                                VARIABLE_TYPE_DOUBLE), //TODO get type the function takes in its args
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
            nextToken = expect_token();
            if (nextToken.has_value() && nextToken.value().text != ";") {
                expressionTokens.push_back(nextToken.value());
            } else {
                break;
            }
        }

        std::stack<llvm::Value *> numbers;
        std::stack<std::string> ops;
        std::map<std::string, int> op_precedence;
        op_precedence["+"] = 10;
        op_precedence["-"] = 10;
        op_precedence["*"] = 20;
        op_precedence["/"] = 20;
        int i = 0;
        for (const Token &exprToken : expressionTokens) {
            if (exprToken.text == "(") {
                ops.push(exprToken.text);
            } else if (exprToken.text == ")") {
                while (!ops.empty() && ops.top() != "(") {
                    llvm::Value *val2 = numbers.top();
                    numbers.pop();

                    llvm::Value *val1 = numbers.top();
                    numbers.pop();

                    std::string op = ops.top();
                    ops.pop();

                    numbers.push(eval(val1, val2, op));
                }

                // pop opening brace.
                ops.pop();
            }

                // if token is a number, add to stack
            else if (!isMathematicalOperator(exprToken.text)) {
                llvm::Value *numberValue = llvm::ConstantInt::get(
                        llvm::IntegerType::getInt32Ty(*LLVMManager::llvmCtx),
                        llvm::APInt(32, std::stoi(exprToken.text)));
                //std::stod(exprToken.text);
                numbers.push(numberValue);
            }
                // Current token is an operator.
            else {

                while (!ops.empty() && op_precedence[ops.top()]
                                       >= op_precedence[exprToken.text]) {
                    llvm::Value *val2 = numbers.top();
                    numbers.pop();

                    llvm::Value *val1 = numbers.top();
                    numbers.pop();

                    std::string op = ops.top();
                    ops.pop();

                    numbers.push(eval(val1, val2, op));
                }

                // Push current token to 'ops'.
                ops.push(exprToken.text);
            }
            i++;
        }

        //Do remaining operations
        while (!ops.empty()) {
            llvm::Value *val2 = numbers.top();
            numbers.pop();

            llvm::Value *val1 = numbers.top();
            numbers.pop();

            std::string op = ops.top();
            ops.pop();

            numbers.push(eval(val1, val2, op));
        }

        // Top of 'numbers' contains result, return
        return numbers.top();
    }

    bool Parser::isMathematicalOperator(const std::string &op) {
        return op == "+" || op == "-" || op == "*" || op == "/";
    }
}