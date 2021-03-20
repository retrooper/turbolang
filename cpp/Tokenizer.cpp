#include "tokenizer/Tokenizer.h"

namespace turbolang {
    std::vector<Token> Tokenizer::tokenize(const std::string &code) {
        std::vector<Token> tokens;
        struct Token currentToken;
        currentToken.lineNumber = 1;
        for (char currChar : code) {
            if (currentToken.type == TOKEN_TYPE_STRING_ESCAPE_SEQUENCE) {
                switch (currChar) {
                    case 't':
                        currentToken.text.append(1, '\t');
                        break;
                    case 'r':
                        currentToken.text.append(1, '\r');
                        break;
                    case '\\':
                        currentToken.text.append(1, '\\');
                        break;
                    default:
                        throw std::runtime_error(std::string("Unknown escape sequence: \\") + std::string(1, currChar) +
                                                 std::string(" in string on line ") +
                                                 std::to_string(currentToken.lineNumber) + std::string("."));
                }
                currentToken.type = TOKEN_TYPE_STRING_LITERAL;
                continue;
            }
            switch (currChar) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (currentToken.type == TOKEN_TYPE_WHITESPACE
                    || currentToken.type == TOKEN_TYPE_POTENTIAL_OPERATOR) {
                        currentToken.type = TOKEN_TYPE_INTEGER_LITERAL;
                        currentToken.text.append(1, currChar);
                    } else if (currentToken.type == TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL) {
                        currentToken.type = TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL;
                        currentToken.text.append(1, currChar);
                    }
                    else {
                        currentToken.text.append(1, currChar);
                    }
                    break;
                case '.':
                    if (currentToken.type == TOKEN_TYPE_WHITESPACE) {
                        currentToken.type = TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL;
                        currentToken.text.append(1, currChar);
                    } else if (currentToken.type == TOKEN_TYPE_INTEGER_LITERAL) {
                        currentToken.type = TOKEN_TYPE_DOUBLE_LITERAL;
                        currentToken.text.append(1, currChar); //was not
                    } else if (currentToken.type == TOKEN_TYPE_STRING_LITERAL) {
                        currentToken.text.append(1, currChar);
                    } else {
                        endToken(&currentToken, &tokens);
                        currentToken.type = TOKEN_TYPE_OPERATOR;
                        currentToken.text.append(1, currChar);
                        endToken(&currentToken, &tokens);
                    }
                    break;

                case '{':
                case '}':
                case '(':
                case ')':
                case '=':
                case ',':
                case ';':
                case '*':
                case '/':
                case '+':
                    if (currentToken.type != TOKEN_TYPE_STRING_LITERAL) {
                        endToken(&currentToken, &tokens);
                        currentToken.type = TOKEN_TYPE_OPERATOR;
                        currentToken.text.append(1, currChar);
                        endToken(&currentToken, &tokens);
                    } else {
                        currentToken.text.append(1, currChar);
                    }
                    break;
                case '-':
                    if (currentToken.type != TOKEN_TYPE_STRING_LITERAL) {
                        currentToken.type = TOKEN_TYPE_POTENTIAL_OPERATOR;
                        currentToken.text.append(1, currChar);
                    } else {
                        currentToken.text.append(1, currChar);
                    }
                    break;

                case ' ':
                case '\t':
                    if (currentToken.type == TOKEN_TYPE_STRING_LITERAL) {
                        currentToken.text.append(1, currChar);
                    } else {
                        endToken(&currentToken, &tokens);
                    }
                    break;
                case '\r':
                case '\n':
                    endToken(&currentToken, &tokens);
                    currentToken.lineNumber++;
                    break;

                case '"':
                    if (currentToken.type != TOKEN_TYPE_STRING_LITERAL) {
                        endToken(&currentToken, &tokens);
                        currentToken.type = TOKEN_TYPE_STRING_LITERAL;
                    } else if (currentToken.type == TOKEN_TYPE_STRING_LITERAL) {
                        endToken(&currentToken, &tokens);
                    }
                    break;

                case '\\':
                    if (currentToken.type == TOKEN_TYPE_STRING_LITERAL) {
                        currentToken.type = TOKEN_TYPE_STRING_LITERAL;
                    } else if (currentToken.type == TOKEN_TYPE_STRING_LITERAL) {
                        endToken(&currentToken, &tokens);
                        currentToken.type = TOKEN_TYPE_OPERATOR;
                        currentToken.text.append(1, currChar);
                        endToken(&currentToken, &tokens);
                    }
                    break;
                default:
                    //IDENTIFIER
                    if (currentToken.type == TOKEN_TYPE_WHITESPACE
                        || currentToken.type == TOKEN_TYPE_INTEGER_LITERAL ||
                        currentToken.type == TOKEN_TYPE_DOUBLE_LITERAL) {
                        endToken(&currentToken, &tokens);
                        currentToken.type = TOKEN_TYPE_IDENTIFIER;
                        currentToken.text.append(1, currChar);
                    } else {
                        currentToken.text.append(1, currChar);
                    }
                    break;
            }
        }
        endToken(&currentToken, &tokens);

        return tokens;
    }

    void Tokenizer::endToken(Token *token, std::vector<turbolang::Token> *tokens) {
        if (token->type != TOKEN_TYPE_WHITESPACE) {
            tokens->push_back(*token);
        } else if (token->type == TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL) {
            if (token->text == ".") {
                token->type = TOKEN_TYPE_OPERATOR;
            } else {
                token->type = TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL; //only 1.

            }
        }
        token->type = TOKEN_TYPE_WHITESPACE;
        token->text.erase();
    }

    void Token::debug() const {
        std::cout << "DEBUG TYPE: " << TOKEN_TYPE_STRINGS[type] << ", Text: " << text << ", at line number: "
                  << lineNumber << std::endl;
    }

}
