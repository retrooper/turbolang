#pragma once

namespace turbolang {
    enum VariableType {
        VARIABLE_TYPE_BYTE = 0, VARIABLE_TYPE_SHORT = 1, VARIABLE_TYPE_INT = 2, VARIABLE_TYPE_LONG = 3,
        VARIABLE_TYPE_FLOAT = 4, VARIABLE_TYPE_DOUBLE = 5,
        VARIABLE_TYPE_BYTE_PTR = 6, VARIABLE_TYPE_SHORT_PTR = 7, VARIABLE_TYPE_INT_PTR = 8, VARIABLE_TYPE_LONG_PTR = 9,
        VARIABLE_TYPE_FLOAT_PTR = 10, VARIABLE_TYPE_DOUBLE_PTR = 11
    };
    //TODO getVariableType(string)

}