#include "data/type.h"

namespace turbolang {
    std::optional<variabletype> get_variable_type(const std::string &name) {
        if (name == "byte") {
            return VARIABLE_TYPE_BYTE;
        } else if (name == "short") {
            return VARIABLE_TYPE_SHORT;
        } else if (name == "int") {
            return VARIABLE_TYPE_INT;
        } else if (name == "long") {
            return VARIABLE_TYPE_LONG;
        } else {
            return std::nullopt;
        }
    }

    std::optional<functiontype> get_function_type(const std::string &name) {
        if (name == "byte") {
            return FUNCTION_TYPE_BYTE;
        } else if (name == "short") {
            return FUNCTION_TYPE_SHORT;
        } else if (name == "int") {
            return FUNCTION_TYPE_INT;
        } else if (name == "long") {
            return FUNCTION_TYPE_LONG;
        } else {
            return std::nullopt;
        }
    }
}
