#include "parser/parser.h"

namespace turbolang {
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
                            std::cout << "PAR TYPE: " << parameterDefinition.type << std::endl;
                        }
                        auto variableName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
                        if (variableName.has_value()) {
                            parameterDefinition.name = variableName.value().text;
                            std::cout << "PAR NAME: " << parameterDefinition.name << std::endl;
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
                    functions[functionDefinition.name] = functionDefinition;
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
        std::optional<statement> statement = parseStatement();
        if (statement.has_value()) {
            statements.push_back(statement.value());
        }

        /*if (!expect_token_type(TOKEN_TYPE_OPERATOR, "}").has_value()) {
            throw std::runtime_error("Unbalanced '}'. Line: " + std::to_string(currentToken->lineNumber));
        }*/
        return statements;
    }

    std::optional<statement> parser::parseStatement() {
        std::vector<token>::iterator startToken;
        struct statement statement;

        auto typeToken = expect_token_type(
                TOKEN_TYPE_IDENTIFIER);//COULD BE VARIABLE DECLARATION OR VARIABLE MODIFICATION OR FUNCTION CALL
        if (typeToken.has_value()) {
            if (get_variable_type(typeToken.value().text).has_value()) {
                //IT IS A VARIABLE DECLARATION
                parseVariableDeclaration(statement, typeToken.value());
            } else {
                auto equalsOperator = expect_token_type(TOKEN_TYPE_OPERATOR, "=");
                if (equalsOperator.has_value()) {
                    //IT IS A VARIABLE MODIFICATION. We pass in variable name
                    parseVariableModification(statement, typeToken);
                } else {
                    //IT IS A FUNCTION CALL. We pass in the function name
                    parseFunctionCall(statement, typeToken);
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
                    auto variableValue = expect_token();
                    if (variableValue.has_value()) {
                        std::cout << "STATEMENT TYPE: " << statement.type << ", VAR TYPE: " << statement.vartype
                                  << ", NAME: "
                                  << statement.name << ", VALUE: " << variableValue.value().text << std::endl;
                        llvm::Value* value = turbolang::compilermanager::llvmIRBuilder.getInt(llvm::APInt(32, std::stoi(variableValue.value().text)));
                        llvm::IntegerType* type = turbolang::compilermanager::llvmIRBuilder.getInt32Ty();
                        turbolang::compilermanager::llvmModule->getOrInsertGlobal(variableNameToken.value().text, type);
                        llvm::GlobalVariable* globalVariable = turbolang::compilermanager::llvmModule->getGlobalVariable(variableNameToken.value().text);
                        std::cout << globalVariable->getName().str() << std::endl;
                        //TODO FINISH, global var created
                    }
                }
            }
        }
    }


    void parser::parseVariableModification(statement &statement,
                                           const std::optional<token> &variableName) {
        auto variableValue = expect_token();
        if (variableValue.has_value()) {
            statement.type = VARIABLE_MODIFICATION;
            //statement.vartype = ?
            statement.name = variableName.value().text;
            std::cout << "MODIFIED VALUE: " << variableValue.value().text << std::endl;
            if (!expect_token_type(TOKEN_TYPE_OPERATOR, ";").has_value()) {
                throw std::runtime_error("Expected a semicolon in variable modification!");
            }
        }
    }

    void parser::parseFunctionCall(statement &statement,
                                   const std::optional<token> &functionName) {

    }

}
