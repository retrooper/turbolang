#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

namespace turbolang {
    enum tokentype {
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

    struct token {
        tokentype type{TOKEN_TYPE_WHITESPACE};
        std::string text;
        size_t lineNumber{0};

        void debug() const;
    };


    class tokenizer {
    public:
        tokenizer() = default;

        std::vector<token> tokenize(const std::string &code);

    private:
        void endToken(token *token, std::vector<turbolang::token> *tokens);
    };
}
