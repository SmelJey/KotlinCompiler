#include "Configuration.h"

Configuration::Configuration() = default;

bool Configuration::GetLexerDebug() const {
    return isLexerDebugOption;
}

bool Configuration::GetParserDebug() const {
    return isParserDebugOption;
}

bool Configuration::GetSemanticsDebug() const {
    return isSemanticsDebugOption;
}

const std::vector<std::string>& Configuration::GetPaths() const {
    return myPaths;
}

