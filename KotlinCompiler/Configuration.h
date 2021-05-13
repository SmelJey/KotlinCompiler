#pragma once

#include <string>
#include <vector>

class Configuration {
public:
    bool GetLexerDebug() const;

    bool GetParserDebug() const;

    const std::vector<std::string>& GetPaths() const;

private:
    Configuration();

    std::vector<std::string> myPaths;

    bool isLexerDebugOption = false;
    bool isParserDebugOption = false;

    friend class ConfigurationBuilder;
};