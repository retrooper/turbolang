#pragma once

#include "tokenizer/tokenizer.h"
#include "utils/printer.h"
#include "utils/exiter.h"
#include "data/functiondefinition.h"
#include "data/parameterdefinition.h"
#include "data/functioncallprocessor.h"
#include "data/type.h"
#include "data/statement.h"
#include <vector>
#include <optional>

namespace turbolang {
    class parser {
        friend class compilermanager;
    public:
        parser();
        void parse(std::vector<token> &tokens);

    private:

        std::vector<token>::iterator currentToken;
        std::vector<token>::iterator endToken;
        std::string currentFuncName;
        bool expect_function_definition();

        std::optional<std::vector<statement>> parse_function_body();
        std::optional<token> expect_token_type(const tokentype &type, const std::string &name);
        std::optional<token> expect_token_type(const tokentype& type);
        std::optional<token> expect_token();
        std::optional<variabletype> expect_token_variable_type();
        std::optional<functiontype> expect_token_function_type();
        std::optional<bool> expect_boolean_value();

        std::optional<statement> parseStatement();

        void parseVariableDeclaration(statement &statement, const token &typeToken);

        void parseVariableModification(statement &statement, const std::optional<token> &variableName);

        llvm::Value * parseFunctionCall(statement &statement, const std::string &functionName);

        void parseWhileLoop(statement& statement);

        void parseReturn(statement& statement);
    };
}