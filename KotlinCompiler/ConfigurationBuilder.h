#pragma once

#include "Configuration.h"

#include <string>
#include <vector>

class ConfigurationBuilder {
public:
    ConfigurationBuilder& SetLexerDebug();
    ConfigurationBuilder& SetParserDebug();
    ConfigurationBuilder& SetSemanticsDebug();
    ConfigurationBuilder& AddPaths(const std::vector<std::string>& paths);

    Configuration Build() const;

private:
    Configuration myConfiguration;
};