#pragma once

#include <vector>
#include <string>

class Configuration {
public:
    bool GetLexerDebug() const;

    bool GetParserDebug() const;

    const std::vector<std::string>& GetPaths() const;

private:
    Configuration();

    std::vector<std::string> myPaths;

    bool isLexerDebugOption;
    bool isParserDebugOption;

    friend class ConfigurationBuilder;
};