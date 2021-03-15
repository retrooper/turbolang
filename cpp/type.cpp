#include "data/type.h"

namespace turbolang {
    std::map<const std::string, variabletype> variableTypeMap = {
            {"byte",  VARIABLE_TYPE_BYTE},
            {"short", VARIABLE_TYPE_SHORT},
            {"int",   VARIABLE_TYPE_INT},
            {"long", VARIABLE_TYPE_LONG},
            {"float", VARIABLE_TYPE_FLOAT},
            {"double", VARIABLE_TYPE_DOUBLE},
            {"string", VARIABLE_TYPE_STRING}
    };
    std::map<const std::string, functiontype> functionTypeMap = {
            {"void", FUNCTION_TYPE_VOID},
            {"byte",  FUNCTION_TYPE_BYTE},
            {"short",  FUNCTION_TYPE_SHORT},
            {"int",    FUNCTION_TYPE_INT},
            {"long",  FUNCTION_TYPE_LONG},
            {"float",  FUNCTION_TYPE_FLOAT},
            {"double",  FUNCTION_TYPE_DOUBLE},
            {"string",  FUNCTION_TYPE_STRING},
    };

    std::optional<variabletype> get_variable_type(const std::string &name) {
        if (variableTypeMap.find(name) == variableTypeMap.end()) {
            return std::nullopt;
        }
        return variableTypeMap[name];
    }

    std::optional<functiontype> get_function_type(const std::string &name) {
        if (functionTypeMap.find(name) == functionTypeMap.end()) {
            return std::nullopt;
        }
        return functionTypeMap[name];
    }
}
