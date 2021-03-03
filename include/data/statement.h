#pragma once
#include "type.h"
#include <string>
#include <vector>
namespace turbolang {
    enum statementtype {
        VARIABLE_DECLARATION, VARIABLE_MODIFICATION, FUNCTION_CALL
    };

    struct statement {
        std::string name;
        variabletype vartype;
        statementtype type;
        std::vector<statement> parameters;
    };
}