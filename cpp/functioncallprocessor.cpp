#include "data/functioncallprocessor.h"

namespace turbolang {

    void functioncallprocessor::process(const turbolang::functiondefinition &currentFunction,
                                        std::vector<llvm::Value *> &arguments) {
        //TODO get function call target name
    }

    void printfcallprocessor::process(const turbolang::functiondefinition &currentFunction,
                                      std::vector<llvm::Value *> &arguments) {
        turbolang::printer::printf(arguments);
    }
}
