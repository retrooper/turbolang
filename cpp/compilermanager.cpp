#include "utils/compilermanager.h"

llvm::LLVMContext turbolang::compilermanager::llvmContext;
llvm::IRBuilder<> turbolang::compilermanager::llvmIRBuilder(llvmContext);
std::unique_ptr<llvm::Module> turbolang::compilermanager::llvmModule;
bool turbolang::compilermanager::output_to_file = true;
std::map<std::string, turbolang::functioncallprocessor *> turbolang::compilermanager::functionCallProcessorMap;
std::map<std::string, turbolang::functiondefinition> turbolang::compilermanager::functions;
void turbolang::compilermanager::prepare() {
    llvmModule = std::make_unique<llvm::Module>("inputFile", llvmContext);
    turbolang::printer::prepare();
    turbolang::exiter::prepare();
    turbolang::sleeper::prepare();
}

void turbolang::compilermanager::generate_byte_code(int exitCode) {
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
    system("clang ../bytecode/output.ll -Ofast");
    std::filesystem::current_path(base_path);
}

void turbolang::compilermanager::execute_binary() {
    system("bash binary/timer.sh ./binary/a.out");
}