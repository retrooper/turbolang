#include "utils/sleeper.h"

namespace turbolang {
    void sleeper::prepare() {
        std::vector<llvm::Type *> args;
        args.push_back(llvm::Type::getInt32Ty(compilermanager::llvmContext));

        bool doesFunctionSizeVary = false;
        llvm::FunctionType *putsType =
                llvm::FunctionType::get(compilermanager::llvmIRBuilder.getInt32Ty(), args, doesFunctionSizeVary);
        llvm::Function::Create(putsType, llvm::Function::ExternalLinkage, "usleep",
                               turbolang::compilermanager::llvmModule.get());
    }

    void sleeper::sleep(llvm::Value *arg) {
        llvm::Value *thousandValue = llvm::ConstantInt::get(compilermanager::llvmIRBuilder.getInt32Ty(), 1000);
        llvm::Value* sleepArg = compilermanager::llvmIRBuilder.CreateSDiv(arg, thousandValue);
        compilermanager::llvmIRBuilder.CreateCall(compilermanager::llvmModule->getFunction("usleep"), sleepArg);
    }
}