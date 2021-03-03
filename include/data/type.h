#pragma once
#include <string>
#include <optional>
namespace turbolang {
    enum variabletype {
        VARIABLE_TYPE_BYTE, VARIABLE_TYPE_SHORT, VARIABLE_TYPE_INT, VARIABLE_TYPE_LONG
    };

    enum functiontype {
        FUNCTION_TYPE_VOID, FUNCTION_TYPE_BYTE, FUNCTION_TYPE_SHORT, FUNCTION_TYPE_INT, FUNCTION_TYPE_LONG
    };

    std::optional<variabletype> get_variable_type(const std::string& name);

    std::optional<functiontype> get_function_type(const std::string& name);
}