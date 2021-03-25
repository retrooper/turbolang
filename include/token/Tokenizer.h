#pragma once
#include "Token.h"
#include <vector>
#include <stdexcept>
#include <iostream>
namespace turbolang {
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

    class Tokenizer {
    public:
        Tokenizer() = default;

        static std::vector<Token> tokenize(const std::string &code);

    private:
        static void endToken(Token *token, std::vector<turbolang::Token> *tokens);
    };
}
