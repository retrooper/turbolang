#include "utils/printer.h"

void turbolang::printer::prepare_printer(){
    /*Declare that printf exists and has signature int (i8*, ...)**/
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt8PtrTy(turbolang::compilermanager::llvmContext));
    /*`true` specifies the function as variadic*/
    llvm::FunctionType *printfType =
            llvm::FunctionType::get(turbolang::compilermanager::llvmIRBuilder.getInt32Ty(), args, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf",
                           turbolang::compilermanager::llvmModule.get());
}

void turbolang::printer::printf(const std::string &format, const std::vector<turbolang::objectwrapper>& values) {
    llvm::Value *printFormat = turbolang::compilermanager::llvmIRBuilder.CreateGlobalStringPtr(format);
    std::vector<llvm::Value*> printArgs;
    printArgs.reserve(1 + values.size());
    printArgs.push_back(printFormat);
    for (const turbolang::objectwrapper& objectWrapper : values) {
        llvm::Value* llvmValue;
        if (objectWrapper.type == OBJECT_TYPE_STRING
        || objectWrapper.type == OBJECT_TYPE_BOOL) {
            std::string str = objectWrapper.type == OBJECT_TYPE_STRING ? objectWrapper.str : std::to_string(objectWrapper.b);
            llvmValue = turbolang::compilermanager::llvmIRBuilder.CreateGlobalStringPtr(str);
        }
        else if (objectWrapper.is_numeric_integer()){
            llvmValue = llvm::ConstantInt::get(turbolang::compilermanager::llvmContext,
                                                              llvm::APInt(objectWrapper.numericBitCount, objectWrapper.get_numeric_integer_value()));
        }
        else if (objectWrapper.is_numeric_floating_point()) {
            llvmValue = llvm::ConstantFP::get(turbolang::compilermanager::llvmContext,
                                              llvm::APFloat(objectWrapper.get_numeric_floating_value()));
        }
        else {
            //what
        }
        printArgs.push_back(llvmValue);
    }
    turbolang::compilermanager::llvmIRBuilder.CreateCall(turbolang::compilermanager::llvmModule->getFunction("printf"), printArgs);
}

void turbolang::printer::print(const std::string &message) {
    std::vector<objectwrapper> objectWrappers = {message.c_str()};
    printf("%s", objectWrappers);
}

void turbolang::printer::println(const std::string &message) {
    print(message + "\n");
}

