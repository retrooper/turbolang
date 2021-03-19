#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

namespace turbolang {
    enum TokenType {
        TOKEN_TYPE_WHITESPACE,
        TOKEN_TYPE_IDENTIFIER,
        TOKEN_TYPE_INTEGER_LITERAL,
        TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL,
        TOKEN_TYPE_DOUBLE_LITERAL,
        TOKEN_TYPE_STRING_LITERAL,
        TOKEN_TYPE_OPERATOR,
        TOKEN_TYPE_STRING_ESCAPE_SEQUENCE
    };

    static const char *TOKEN_TYPE_STRINGS[] = {
            "TOKEN_TYPE_WHITESPACE",
            "TOKEN_TYPE_IDENTIFIER",
            "TOKEN_TYPE_INTEGER_LITERAL",
            "TOKEN_TYPE_POTENTIAL_DOUBLE_LITERAL",
            "TOKEN_TYPE_DOUBLE_LITERAL",
            "TOKEN_TYPE_STRING_LITERAL",
            "TOKEN_TYPE_OPERATOR",
            "TOKEN_TYPE_STRING_ESCAPE_SEQUENCE",
            "TOKEN_TYPE_COMMENT"
    };

    struct Token {
        TokenType type{TOKEN_TYPE_WHITESPACE};
        std::string text;
        size_t lineNumber{0};

        void debug() const;
    };


    class Tokenizer {
    public:
        Tokenizer() = default;

        std::vector<Token> tokenize(const std::string &code);

    private:
        void endToken(Token *token, std::vector<turbolang::Token> *tokens);
    };
}
