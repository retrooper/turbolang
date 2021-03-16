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
    turbolang::scanner::prepare();
}

void turbolang::compilermanager::generate_byte_code(int exitCode) {

    llvm::raw_fd_ostream *outputStream = &llvm::outs();
    if (output_to_file) {
        auto current_path = std::filesystem::current_path().string() + "/build";
        std::filesystem::current_path(current_path);
        std::string new_path = current_path + "/bytecode";
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
    system("clang ../bytecode/output.ll -Ofast -o output.out");
    std::filesystem::current_path(base_path);
}

volatile int turbolang::compilermanager::tasksFinished = 0;
void turbolang::compilermanager::generate_executables() {
    auto base_path = std::filesystem::current_path();
    int totalTasks = 4;
    /*
     * WINDOWS
     */
    auto windowsPath = base_path.string() + "/executables/windows";
    std::filesystem::current_path(windowsPath);
    std::thread x86WindowsThread([&windowsPath](){
        //x86
        auto x86WindowsPath = windowsPath + "/x86";
        std::string path = "i686-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x86WindowsPath + "/output_x86.exe";
        system(path.c_str());
        tasksFinished++;
    });

    //x64
    std::thread x64WindowsThread([&windowsPath]() {
        auto x64WindowsPath = windowsPath + "/x64";
        std::string path = "x86_64-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x64WindowsPath + "/output_x64.exe";
        system(path.c_str());
        tasksFinished++;
    });
    /*
     * LINUX
     */
    auto linuxPath = base_path.string() + "/executables/linux";
    //x86
    std::thread x86LinuxThread([&linuxPath](){
        //x86
        auto x86LinuxPath = linuxPath + "/x86";
        std::string path = "clang++ ../../bytecode/output.ll -o " + x86LinuxPath + "/output_x86";
        system(path.c_str());
        tasksFinished++;
    });
    //x64
    std::thread x64LinuxThread([&linuxPath]() {
        auto x64LinuxPath = linuxPath + "/x64";
        std::string path = "clang++ ../../bytecode/output.ll -o " + x64LinuxPath + "/output_x64";
        system(path.c_str());
        tasksFinished++;
    });

    while (tasksFinished < totalTasks) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    x86WindowsThread.join();
    x86LinuxThread.join();
    x64WindowsThread.join();
    x64LinuxThread.join();
    std::filesystem::current_path(base_path);
}

void turbolang::compilermanager::execute_binary() {
    system("bash binary/timer.sh ./binary/output.out");
}