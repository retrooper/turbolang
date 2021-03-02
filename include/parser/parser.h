#pragma once

#include "tokenizer/tokenizer.h"
#include "utils/printer.h"
#include "utils/exiter.h"
#include <vector>
#include <optional>

namespace turbolang {
    class parser {
    public:
        parser() = default;

        void parse(std::vector<token> &tokens);

    private:
        std::vector<token>::iterator currentToken;
        std::vector<token>::iterator endToken;

        void handle_function_call();
        void handle_variable_declaration();
        bool expect_function_definition();
        std::optional<token> expect_token_type(const tokentype &type, const std::string &name);
        std::optional<token> expect_token_type(const tokentype& type);
        std::optional<token> expect_token();
    };
}