#include "utils/SourceCodeReader.h"

namespace turbolang {
    SourceCodeReaderResult SourceCodeReader::readAndFilterCode(std::ifstream &file, std::string *filteredCode) {
        *filteredCode = "";
        if (!file) {
            return SOURCE_READER_RESULT_FAILURE;
        }
        std::string line;
        if (file.is_open()) {
            while (getline(file, line)) {
                bool commentFound = false;
                for (const char &character : line) {
                    if (character == ' ') {
                        continue;
                    }
                    if (character == '#') {
                        commentFound = true;
                        break;
                    }
                }
                if (!commentFound) {
                    *filteredCode += line;
                }
            }
            file.close();
        }
        return SOURCE_READER_RESULT_SUCCESS;
    }
}