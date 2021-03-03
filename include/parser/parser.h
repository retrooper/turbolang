#pragma once

#include "tokenizer/tokenizer.h"
#include "utils/printer.h"
#include "utils/exiter.h"
#include "data/functiondefinition.h"
#include "data/parameterdefinition.h"
#include "data/type.h"
#include "data/statement.h"
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
        std::map<std::string, functiondefinition> functions;
        bool expect_function_definition();
        std::optional<std::vector<statement>> parse_function_body();
        std::optional<token> expect_token_type(const tokentype &type, const std::string &name);
        std::optional<token> expect_token_type(const tokentype& type);
        std::optional<token> expect_token();

        std::optional<statement> parseStatement();

        void parseVariableDeclaration(statement &statement, const token &typeToken);

        void parseVariableModification(statement &statement, const std::optional<token> &variableName);

        void parseFunctionCall(statement &statement, const std::optional<token> &functionName);
    };
}