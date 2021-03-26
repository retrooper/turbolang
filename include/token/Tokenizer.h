#pragma once
#include "Token.h"
#include "math/MathEvaluator.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <optional>
namespace turbolang {
    static const char *TOKEN_TYPE_STRINGS[] = {
            "TOKEN_TYPE_WHITESPACE",
            "TOKEN_TYPE_IDENTIFIER",
            "TOKEN_TYPE_RETURN",
            "TOKEN_TYPE_WHILE",
            "TOKEN_TYPE_IF",
            "TOKEN_TYPE_ELSE_IF",
            "TOKEN_TYPE_ELSE",
            "TOKEN_TYPE_INTEGER_LITERAL",
            "TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL",
            "TOKEN_TYPE_DOUBLE_LITERAL",
            "TOKEN_TYPE_STRING_LITERAL",
            "TOKEN_TYPE_OPERATOR",
            "TOKEN_TYPE_POTENTIAL_OPERATOR",
            "TOKEN_TYPE_STRING_ESCAPE_SEQUENCE"
    };

    class Tokenizer {
    public:
        Tokenizer() = default;

        static std::vector<Token> tokenize(const std::string &code);

    private:
        static void endToken(Token *token, std::vector<turbolang::Token> *tokens);
    };
}
