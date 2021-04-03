#include "builder/Builder.h"

namespace turbolang {
    volatile int Builder::tasksFinished = 0;

    void Builder::buildExecutables() {
        /**
         * LOCAL BINARY FILE
         */
        auto bytecodePath = std::filesystem::current_path();
        auto basePath = bytecodePath.string() + "/../";

        std::string binaryPath = basePath + "/binary";
        std::filesystem::current_path(binaryPath);
        std::system("clang ../bytecode/output.ll -Ofast -lm -o output.out");
        std::filesystem::current_path(basePath);

        tasksFinished = 0;
        /**
         * Cross compile for other platforms.
         */

        //Cross compile for Windows
        auto windowsPath = basePath + "/executables/windows";
        std::filesystem::current_path(windowsPath);
        //x86 Windows
        std::thread x86WindowsThread([&windowsPath]() {
            auto x86WindowsPath = windowsPath + "/x86";
            std::string command =
                    "i686-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x86WindowsPath + "/output_x86.exe";
            std::system(command.c_str());
            tasksFinished++;
        });

        //x86_64 Windows
        std::thread x64WindowsThread([&windowsPath]() {
            auto x64WindowsPath = windowsPath + "/x64";
            std::string command =
                    "x86_64-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x64WindowsPath + "/output_x64.exe";
            std::system(command.c_str());
            tasksFinished++;
        });

        //Cross compile for Linux
        auto linuxPath = basePath + "/executables/linux";
        //x86 Linux
        std::thread x86LinuxThread([&linuxPath]() {
            auto x86LinuxPath = linuxPath + "/x86";
            std::string command = "clang++ ../../bytecode/output.ll -o " + x86LinuxPath + "/output_x86";
            std::system(command.c_str());
            tasksFinished++;
        });
        //x86_64 Linux
        std::thread x64LinuxThread([&linuxPath]() {
            auto x64LinuxPath = linuxPath + "/x64";
            std::string command = "clang++ ../../bytecode/output.ll -o " + x64LinuxPath + "/output_x64";
            std::system(command.c_str());
            tasksFinished++;
        });

        //Cross compile for MacOS
        auto macosPath = basePath + "/executables/macos";
        //TODO arm7

        //arm64 MacOS
        std::thread arm64MacosThread([&macosPath]() {
            auto arm64MacosPath = macosPath + "/arm64";
            std::string command = "clang++ ../../bytecode/output.ll -o " + arm64MacosPath + "/output_arm64.app";
            std::system(command.c_str());
            tasksFinished++;
        });
        //arm64e (newer than arm64) MacOS
        std::thread arm64eMacosThread([&macosPath]() {
            auto arm64eMacosPath = macosPath + "/arm64e";
            std::string command = "clang++ ../../bytecode/output.ll -o " + arm64eMacosPath + "/output_arm64e.app";
            std::system(command.c_str());
            tasksFinished++;
        });

        int totalTasks = 6;//6 threads we just created...
        while (tasksFinished < totalTasks) {
            ;
        }
        /**
         * Ensure we don't interrupt them. They MUST
         */
        //WINDOWS
        x86WindowsThread.join();
        x64WindowsThread.join();
        //LINUX
        x86LinuxThread.join();
        x64LinuxThread.join();
        //MACOS
        arm64MacosThread.join();
        arm64eMacosThread.join();
        //We can go back to the base path.
        std::filesystem::current_path(basePath);
    }

}