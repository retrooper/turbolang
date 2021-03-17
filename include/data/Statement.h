#pragma once

#include "variable/Variable.h"
#include <string>
#include <vector>

namespace turbolang {
    enum statementtype {
        VARIABLE_DECLARATION, VARIABLE_MODIFICATION, FUNCTION_CALL
    };

    struct Statement {
        std::string name;
        statementtype type;
        std::vector<Statement> parameters;
    };
}