#pragma once

#include "Configuration.h"

#include <vector>
#include <string>

class ConfigurationBuilder {
public:
    ConfigurationBuilder& SetLexerDebug();
    ConfigurationBuilder& SetParserDebug();
    ConfigurationBuilder& AddPaths(const std::vector<std::string>& paths);

    Configuration Build() const;

private:
    Configuration myConfiguration;
};