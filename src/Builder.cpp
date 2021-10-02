#include "builder/Builder.h"

namespace turbolang {
    volatile int Builder::tasksFinished = 0;
    std::vector<std::string> Builder::libraries;

    void Builder::buildExecutables() {
        std::string result;
        //TODO remove duplicate libraries
        for (auto const &s : libraries) {
            result += ", " + s;
        }
        if (!libraries.empty()) {
            result = result.substr(2);
        } else {
            result = "None";
        }
        LOG_INFO("Linking against C libraries: " << result);
        std::string additionalArguments = "-Ofast ";//Ofast is maximum optimizations, O0 is no optimizations
        //TODO remove duplicate libraries
        for (const std::string &library : libraries) {
            additionalArguments += "-l" + library + " ";
        }
        /**
         * LOCAL BINARY FILE
         */
        auto bytecodePath = std::filesystem::current_path();
        auto basePath = bytecodePath.string() + "/../";

        std::string binaryPath = basePath + "/binary";
        if (!std::filesystem::exists(binaryPath)) {
            std::filesystem::create_directory(binaryPath);
        }
        std::filesystem::current_path(binaryPath);
        std::system((std::string("clang ../bytecode/output.ll -o output.out ") + additionalArguments).c_str());
        std::filesystem::current_path(basePath);

        tasksFinished = 0;
        /**
         * Cross compile for other platforms.
         */

        //Cross compile for Windows

        auto windowsPath = basePath + "/executables/windows";
        if (!std::filesystem::exists(windowsPath)) {
            std::filesystem::create_directories(windowsPath);
        }
        std::filesystem::current_path(windowsPath);
        //x86 Windows
        std::thread x86WindowsThread([&windowsPath, &additionalArguments]() {
            auto x86WindowsPath = windowsPath + "/x86";
            if (!std::filesystem::exists(x86WindowsPath)) {
                std::filesystem::create_directories(x86WindowsPath);
            }
            std::string command =
                    "i686-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x86WindowsPath + "/output_x86.exe";
            command += additionalArguments;
            std::system(command.c_str());
            tasksFinished++;
        });

        //x86_64 Windows
        std::thread x64WindowsThread([&windowsPath, &additionalArguments]() {
            auto x64WindowsPath = windowsPath + "/x64";
            if (!std::filesystem::exists(x64WindowsPath)) {
                std::filesystem::create_directories(x64WindowsPath);
            }
            std::string command =
                    "x86_64-w64-mingw32-clang++ ../../bytecode/output.ll -o " + x64WindowsPath + "/output_x64.exe " + additionalArguments;
            std::system(command.c_str());
            tasksFinished++;
        });

        //Cross compile for Linux
        auto linuxPath = basePath + "/executables/linux";
        if (!std::filesystem::exists(linuxPath)) {
            std::filesystem::create_directories(linuxPath);
        }
        //x86 Linux
        std::thread x86LinuxThread([&linuxPath, &additionalArguments]() {
            auto x86LinuxPath = linuxPath + "/x86";
            if (!std::filesystem::exists(x86LinuxPath)) {
                std::filesystem::create_directories(x86LinuxPath);
            }
            std::string command = "clang++ ../../bytecode/output.ll -o " + x86LinuxPath + "/output_x86 " + additionalArguments;
            std::system(command.c_str());
        });
        //x86_64 Linux
        std::thread x64LinuxThread([&linuxPath, &additionalArguments]() {
            auto x64LinuxPath = linuxPath + "/x64";

            if (!std::filesystem::exists(x64LinuxPath)) {
                std::filesystem::create_directories(x64LinuxPath);
            }
            std::string command = "clang++ ../../bytecode/output.ll -o " + x64LinuxPath + "/output_x64 " + additionalArguments;
            std::system(command.c_str());
        });

        //Cross compile for MacOS
        auto macosPath = basePath + "/executables/macos";
        if (!std::filesystem::exists(macosPath)) {
            std::filesystem::create_directories(macosPath);
        }
        //TODO arm7

        //arm64 MacOS
        std::thread arm64MacosThread([&macosPath, &additionalArguments]() {
            auto arm64MacosPath = macosPath + "/arm64";
            if (!std::filesystem::exists(arm64MacosPath)) {
                std::filesystem::create_directories(arm64MacosPath);
            }
            std::string command = "clang++ ../../bytecode/output.ll -o " + arm64MacosPath + "/output_arm64.app " + additionalArguments;
            std::system(command.c_str());
        });
        //arm64e (newer than arm64) MacOS
        std::thread arm64eMacosThread([&macosPath, &additionalArguments]() {
            auto arm64eMacosPath = macosPath + "/arm64e";
            if (!std::filesystem::exists(arm64eMacosPath)) {
                std::filesystem::create_directories(arm64eMacosPath);
            }
            std::string command = "clang++ ../../bytecode/output.ll -o " + arm64eMacosPath + "/output_arm64e.app " + additionalArguments;
            std::system(command.c_str());
        });
        /**
         * Ensure we don't interrupt them. They MUST finish
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