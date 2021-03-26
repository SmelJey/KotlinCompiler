#include "ConfigurationBuilder.h"

ConfigurationBuilder& ConfigurationBuilder::AddPaths(const std::vector<std::string>& paths) {
    myConfiguration.myPaths = paths;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::SetLexerDebug() {
    myConfiguration.isLexerDebugOption = true;
    return *this;
}

Configuration ConfigurationBuilder::Build() const {
    return myConfiguration;
}
