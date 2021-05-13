#include "Configuration.h"

Configuration::Configuration() = default;

bool Configuration::GetLexerDebug() const {
    return isLexerDebugOption;
}

bool Configuration::GetParserDebug() const {
    return isParserDebugOption;
}

const std::vector<std::string>& Configuration::GetPaths() const {
    return myPaths;
}

