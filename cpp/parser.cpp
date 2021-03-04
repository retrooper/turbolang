#include "parser/parser.h"

namespace turbolang {

    parser::parser() {
        turbolang::compilermanager::functionCallProcessorMap["printf"] = new turbolang::printfcallprocessor();
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
            break; //was not
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

    bool parser::expect_function_definition() {
        std::vector<token>::iterator parseStart = currentToken;
        auto functionType = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (functionType.has_value() && functionType.value().text == "void") {
            auto functionName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
            if (functionName.has_value() && functionName.value().text == "main") {
                auto openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
                if (openingParenthesis.has_value()) {
                    functiondefinition functionDefinition;
                    functionDefinition.name = functionName.value().text;

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

                    std::optional<std::vector<statement>> statements = parse_function_body();
                    if (!statements.has_value()) {
                        currentToken = parseStart;
                        return false;
                    }
                    turbolang::compilermanager::functions[functionDefinition.name] = functionDefinition;
                    currentFuncName = functionDefinition.name;
                    return true;
                } else {
                    std::cout << currentToken->text << std::endl;
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
            if (get_variable_type(typeToken.value().text).has_value()) { //TODO support custom classes as types
                //IT IS A VARIABLE DECLARATION
                parseVariableDeclaration(statement, typeToken.value());
                return statement;
            } else {
                auto equalsOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "=");
                if (equalsOperator.has_value()) {
                    //IT IS A VARIABLE MODIFICATION. We pass in variable name
                    parseVariableModification(statement, typeToken);
                    return statement;
                } else {
                    //IT IS A FUNCTION CALL. We pass in the function name
                    parseFunctionCall(statement, typeToken);
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
                        switch (variableValueToken.value().type) {
                            case TOKEN_TYPE_IDENTIFIER:
                                allocaInst = turbolang::compilermanager::functions[currentFuncName].allocaMap[variableValueToken.value().text];
                                allocatedValue = turbolang::compilermanager::functions[currentFuncName].variableMap[variableValueToken.value().text];
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
                        if (allocaInst == nullptr) {
                            allocaInst = turbolang::compilermanager::llvmIRBuilder.CreateAlloca(type, arraySize, twine);
                        }
                        if (allocatedValue == nullptr) {
                            allocatedValue = allocaInst->getOperand(
                                    0);
                        }
                        turbolang::compilermanager::functions[currentFuncName].variableMap[variableNameToken.value().text] = allocatedValue;
                        turbolang::compilermanager::functions[currentFuncName].allocaMap[variableNameToken.value().text] = allocaInst;
                        if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
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
            llvm::Value* val = nullptr;
            switch (variableValueToken.value().type) {
                case TOKEN_TYPE_IDENTIFIER:
                    type = turbolang::compilermanager::functions[currentFuncName].variableMap[variableValueToken.value().text]->getType();
                    val = turbolang::compilermanager::functions[currentFuncName].variableMap[variableValueToken.value().text];
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
            auto allocaInst = turbolang::compilermanager::functions[currentFuncName].allocaMap[variableNameToken.value().text];
            if (val == nullptr) {
            auto storeInst = turbolang::compilermanager::llvmIRBuilder.CreateStore(
                    arraySize, allocaInst,
                    false);

                val = storeInst->getOperand(0);
            }
            turbolang::compilermanager::functions[currentFuncName].variableMap[variableNameToken.value().text] = val;
            if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    void parser::parseFunctionCall(statement &statement,
                                   const std::optional<token> &functionName) {
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

            std::vector<llvm::Value *> llvmFunctionArguments;
            for (const auto &argument : arguments) {
                llvm::Value *llvmFunctionArgument = nullptr;
                switch (argument.type) {
                    case TOKEN_TYPE_IDENTIFIER:
                        llvmFunctionArgument = turbolang::compilermanager::functions[currentFuncName].variableMap[argument.text];
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
            turbolang::compilermanager::functionCallProcessorMap[functionName.value().text]->process(
                    turbolang::compilermanager::functions[currentFuncName], llvmFunctionArguments);
        }
    }
}
