#pragma once

#include <unordered_set>

#include "Lexeme.h"

class ILexer {
public:
    enum class CharGroup {
        Digit,
        Alphabetic,
        Operation,
        Brace,
        Spacing,
        Unknown,
        EndOfFile,
    };

    static const std::unordered_set<char> BraceCharset;
    static const std::unordered_set<char> OperationsCharset;
    static const std::unordered_set<char> SpacingCharset;
    static const std::unordered_set<char> NewlineCharset;
    static const std::unordered_set<std::string> OperationsSets[3];

    static CharGroup GetCharGroup(int character);

    ILexer(const ILexer& src) = delete;
    virtual ~ILexer() = default;

    void Init();

    Lexeme GetLexeme() const;
    Lexeme NextLexeme();
protected:
    ILexer();
private:
    Lexeme myCurrentLexeme;

    virtual Lexeme GetLexemeFromInput() = 0;
};
