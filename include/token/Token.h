#pragma once
#include <string>
namespace turbolang {
    enum TokenType {
        //Tokens with this type won't be processed.
        TOKEN_TYPE_WHITESPACE,
        //Any text
        TOKEN_TYPE_IDENTIFIER,
        /*
         * Important keywords
         */
        TOKEN_TYPE_CLASS,
        TOKEN_TYPE_DECLARE,
        TOKEN_TYPE_IMPORT,
        TOKEN_TYPE_RETURN,
        TOKEN_TYPE_WHILE,
        TOKEN_TYPE_IF,
        TOKEN_TYPE_ELSE_IF,
        TOKEN_TYPE_ELSE,
        //Literal integer
        TOKEN_TYPE_INTEGER_LITERAL,
        TOKEN_TYPE_DOUBLE_LITERAL_POTENTIAL,
        //Literal floating point.
        TOKEN_TYPE_DOUBLE_LITERAL,
        //Literal string
        TOKEN_TYPE_STRING_LITERAL,
        //Operator(not necessarily a math operator)
        TOKEN_TYPE_OPERATOR,
        TOKEN_TYPE_POTENTIAL_OPERATOR,
        //String escape sequence
        TOKEN_TYPE_STRING_ESCAPE_SEQUENCE
    };

    struct Token {
        TokenType type{TOKEN_TYPE_WHITESPACE};
        std::string text;
        unsigned int lineNumber{0};

        void debug() const;
    };
}