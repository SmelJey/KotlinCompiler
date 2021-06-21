#include "ConfigurationBuilder.h"

ConfigurationBuilder& ConfigurationBuilder::AddPaths(const std::vector<std::string>& paths) {
    myConfiguration.myPaths = paths;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::SetLexerDebug() {
    myConfiguration.isLexerDebugOption = true;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::SetParserDebug() {
    myConfiguration.isParserDebugOption = true;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::SetSemanticsDebug() {
    myConfiguration.isSemanticsDebugOption = true;
    return *this;
}

Configuration ConfigurationBuilder::Build() const {
    return myConfiguration;
}
