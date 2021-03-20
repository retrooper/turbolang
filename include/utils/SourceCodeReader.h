#pragma once

#include <string>
#include <fstream>
#include <filesystem>

namespace turbolang {
    enum SourceCodeReaderResult {
        SOURCE_READER_RESULT_FAILURE = 0, SOURCE_READER_RESULT_SUCCESS = 1
    };

    struct SourceCodeReader {
        static SourceCodeReaderResult readAndFilterCode(std::ifstream &file, std::string *filteredCode);
    };
}