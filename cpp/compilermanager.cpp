#include "utils/compilermanager.h"
#include "utils/printer.h"
#include "utils/exiter.h"
#include "tokenizer/tokenizer.h"

llvm::LLVMContext turbolang::compilermanager::llvmContext;
llvm::IRBuilder<> turbolang::compilermanager::llvmIRBuilder(llvmContext);
std::unique_ptr<llvm::Module> turbolang::compilermanager::llvmModule;
llvm::FunctionType *turbolang::compilermanager::llvmMainFunctionType = nullptr;
llvm::Function *turbolang::compilermanager::llvmMainFunction = nullptr;
bool turbolang::compilermanager::output_to_file = true;
std::map<std::string, turbolang::functioncallprocessor *> turbolang::compilermanager::functionCallProcessorMap;
std::map<std::string, turbolang::functiondefinition> turbolang::compilermanager::functions;
void turbolang::compilermanager::prepare() {
    llvmModule = std::make_unique<llvm::Module>("inputFile", llvmContext);
    turbolang::printer::prepare_printer();
    turbolang::exiter::prepare_exiter();
    /*begin codegen for `main`*/
    llvmMainFunctionType = llvm::FunctionType::get(llvmIRBuilder.getInt32Ty(), false);
    llvmMainFunction = llvm::Function::Create(llvmMainFunctionType, llvm::Function::ExternalLinkage, "main",
                                              llvmModule.get());
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvmContext, "entry", llvmMainFunction);
    llvmIRBuilder.SetInsertPoint(entry);
}

void turbolang::compilermanager::generate_byte_code(int exitCode) {
    /*return value for `main`*/
    llvmIRBuilder.CreateRet(llvm::ConstantInt::get(llvmContext, llvm::APInt(32, exitCode)));
    llvm::raw_fd_ostream *outputStream = &llvm::outs();
    if (output_to_file) {
        auto current_path = std::filesystem::current_path();
        std::string new_path = current_path.string() + "/bytecode";
        std::filesystem::current_path(new_path);
        const char *ouputFileName = "output.ll";
        std::error_code EC;
        outputStream = new llvm::raw_fd_ostream(ouputFileName, EC);
        llvmModule->print(*outputStream, nullptr);
        if (outputStream != &llvm::outs()) {
            delete outputStream;
        }
    } else {
        llvmModule->print(*outputStream, nullptr);
    }
}

void turbolang::compilermanager::cleanup_byte_code(class parser &parser) {
    for (const auto &any : turbolang::compilermanager::functionCallProcessorMap) {
        auto processor = any.second;
        delete processor;
    }
}

void turbolang::compilermanager::generate_binary() {
    auto bytecode_path = std::filesystem::current_path();
    std::string base_path = bytecode_path.string() + "/../";

    std::string binary_path = base_path + "/binary";
    std::filesystem::current_path(binary_path);
    system("clang ../bytecode/output.ll");
    std::filesystem::current_path(base_path);
}

void turbolang::compilermanager::execute_binary() {
    system("./binary/a.out");
}