#include "parser/parser.h"

namespace turbolang {
    void parser::parse(std::vector<token> &tokens) {
        endToken = tokens.end();
        currentToken = tokens.begin();
        //MAIN FUNCTION
        while (currentToken != endToken) {
            if (expect_function_definition()) {
                std::optional<token> firstToken = expect_token();
                if (firstToken.has_value()) {
                    if (firstToken->type == TOKEN_TYPE_IDENTIFIER) {
                        std::optional<token> secondToken = expect_token();
                        currentToken--;
                        currentToken--;
                        if (secondToken->type == TOKEN_TYPE_OPERATOR) {
                            handle_function_call();
                        } else if (secondToken->type == TOKEN_TYPE_IDENTIFIER) {
                            handle_variable_declaration();
                        }
                    }

                } else {
                    std::cout << "Unknown token " << currentToken->text << std::endl;
                }
            } else {
                std::cerr << "Unknown identifier " << currentToken->text << std::endl;
                currentToken++;
            }
            break; //was not
        }
    }

    void parser::handle_function_call() {
        std::optional<token> functionName = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        if (functionName.has_value()) {
            //std::cout << "NAME: " << functionName.value().text << std::endl;
            std::optional<token> openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR, "(");
            std::vector<token> functionArguments;
            if (openingParenthesis.has_value()) {
                //RESOLVING FUNCTION ARGUMENTS
                while (true) {
                    std::optional<token> nextToken = expect_token();
                    if (nextToken->type == TOKEN_TYPE_OPERATOR) {
                        if (nextToken->text == ")") {
                            break;
                        } else if (nextToken->text == ",") {
                            continue;
                        }
                    }
                    if (nextToken.has_value()) {
                        functionArguments.push_back(nextToken.value());
                        //std::cout << "ARGUMENT: " << nextToken->to_string() << std::endl;
                    }
                }

                //FINISHED RESOLVING FUNCTION ARGUMENTS
                if (functionName.value().text == "printf") {
                    std::vector<objectwrapper> printfValues;
                    if (functionArguments.size() != 1) {
                        printfValues.reserve(functionArguments.size() - 1);
                        for (int i = 1; i < functionArguments.size(); i++) {
                            const token argument = functionArguments[i];
                            switch (argument.type) {
                                case TOKEN_TYPE_INTEGER_LITERAL:
                                    printfValues.emplace_back(std::stoi(argument.text));
                                    break;
                                case TOKEN_TYPE_DOUBLE_LITERAL:
                                    printfValues.emplace_back(std::stod(argument.text));
                                    break;
                                case TOKEN_TYPE_STRING_LITERAL:
                                    printfValues.emplace_back(argument.text.c_str());
                                    break;
                                case TOKEN_TYPE_WHITESPACE:
                                    break;
                                case TOKEN_TYPE_IDENTIFIER:
                                    break;
                                case TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL:
                                    break;
                                case TOKEN_TYPE_OPERATOR:
                                    break;
                                case TOKEN_TYPE_STRING_ESCAPE_SEQUENCE:
                                    break;
                                case TOKEN_TYPE_COMMENT:
                                    break;
                            }
                        }
                    }
                    turbolang::printer::printf(functionArguments[0].text, printfValues);
                }
                else if (functionName.value().text == "print") {
                    turbolang::printer::print(functionArguments[0].text);
                }
                else if (functionName.value().text == "println") {
                    turbolang::printer::println(functionArguments[0].text);
                }
            }
        }
    }

    void parser::handle_variable_declaration() {
        std::optional<token> variableType = expect_token_type(TOKEN_TYPE_IDENTIFIER);
        std::cout << "VAR TYPE: " << variableType.value().text << std::endl;
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
                auto openingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR);
                auto closingParenthesis = expect_token_type(TOKEN_TYPE_OPERATOR);
                auto openingCurlyBrackets = expect_token_type(TOKEN_TYPE_OPERATOR);
                if (openingParenthesis.has_value() && closingParenthesis.has_value() &&
                    openingCurlyBrackets.has_value()) {
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
}
