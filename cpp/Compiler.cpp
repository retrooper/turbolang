#include "utils/Compiler.h"

void turbolang::Compiler::generate_byte_code() {
    llvm::raw_fd_ostream *outputStream = &llvm::outs();
        auto current_path = std::filesystem::current_path().string() + "/build";
        std::filesystem::current_path(current_path);
        std::string new_path = current_path + "/bytecode";
        std::filesystem::current_path(new_path);
        const char *ouputFileName = "output.ll";
        std::error_code EC;
        outputStream = new llvm::raw_fd_ostream(ouputFileName, EC);
        if (EC.message() != "Success") {
            std::cout << "LLVM Compilation error code: " << EC.message() << std::endl;
        }
        LLVMManager::llvmModule->print(*outputStream, nullptr);
        if (outputStream != &llvm::outs()) {
            delete outputStream;
        }
}

void turbolang::Compiler::generate_binary() {
    auto bytecode_path = std::filesystem::current_path();
    std::string base_path = bytecode_path.string() + "/../";

    std::string binary_path = base_path + "/binary";
    std::filesystem::current_path(binary_path);
    std::system("clang ../bytecode/output.ll -Ofast -lm -o output.out");
    std::filesystem::current_path(base_path);
}

volatile int turbolang::Compiler::tasksFinished = 0;
void turbolang::Compiler::generate_executables() {
    auto base_path = std::filesystem::current_path();
    /*
     * WINDOWS
     */
    auto windowsPath = base_path.string() + "/executables/windows";
    std::filesystem::current_path(windowsPath);
    std::thread x86WindowsThread([&windowsPath](){
        //x86
        auto x86WindowsPath = windowsPath + "/x86";
        std::string path = "i686-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x86WindowsPath + "/output_x86.exe";
        std::system(path.c_str());
        tasksFinished++;
    });

    //x64
    std::thread x64WindowsThread([&windowsPath]() {
        auto x64WindowsPath = windowsPath + "/x64";
        std::string path = "x86_64-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x64WindowsPath + "/output_x64.exe";
        std::system(path.c_str());
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
        std::system(path.c_str());
        tasksFinished++;
    });
    //x64
    std::thread x64LinuxThread([&linuxPath]() {
        auto x64LinuxPath = linuxPath + "/x64";
        std::string path = "clang++ ../../bytecode/output.ll -o " + x64LinuxPath + "/output_x64";
        std::system(path.c_str());
        tasksFinished++;
    });

    int totalTasks = 4;
    while (tasksFinished < totalTasks) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    x86WindowsThread.join();
    x86LinuxThread.join();
    x64WindowsThread.join();
    x64LinuxThread.join();
    std::filesystem::current_path(base_path);
}

void turbolang::Compiler::execute_binary() {

    //std::system("bash binary/timer.sh ./binary/output.out");
    std::system("./binary/output.out");
}