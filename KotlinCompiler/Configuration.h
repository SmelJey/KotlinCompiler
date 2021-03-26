#pragma once

#include <vector>
#include <string>

class Configuration {
public:
    bool GetLexerDebug() const;

    const std::vector<std::string>& GetPaths() const;

private:
    Configuration();

    std::vector<std::string> myPaths;

    bool isLexerDebugOption;

    friend class ConfigurationBuilder;
};