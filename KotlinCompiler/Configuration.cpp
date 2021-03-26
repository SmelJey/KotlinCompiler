#include "Configuration.h"

Configuration::Configuration() : isLexerDebugOption() {} 

bool Configuration::GetLexerDebug() const {
    return isLexerDebugOption;
}

const std::vector<std::string>& Configuration::GetPaths() const {
    return myPaths;
}

