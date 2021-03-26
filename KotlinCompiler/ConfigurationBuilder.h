#pragma once

#include <vector>
#include <string>
#include "Configuration.h"

class ConfigurationBuilder {
public:
    ConfigurationBuilder& SetLexerDebug();
    ConfigurationBuilder& AddPaths(const std::vector<std::string>& paths);

    Configuration Build() const;

private:
    Configuration myConfiguration;
};