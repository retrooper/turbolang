#include "utils/sleeper.h"

void turbolang::sleeper::sleep(long ms) {
    /*Declare that printf exists and has signature int (i8*, ...)**/
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt64PtrTy(turbolang::compilermanager::llvmContext));
    /*`true` specifies the function as variadic*/
    llvm::FunctionType *msChronoType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getCurrentFunctionReturnType(),  args, true);

    llvm::Function::Create(msChronoType, llvm::Function::ExternalLinkage, "milliseconds",
                           turbolang::compilermanager::llvmModule.get());

    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
