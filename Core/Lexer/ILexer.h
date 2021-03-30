#pragma once

#include <unordered_set>

#include "Lexeme.h"

using LexType = Lexeme::LexemeType;

class ILexer {
public:
    enum class CharGroup : int {
        Digit = 0,
        Alphabetic = 1,
        Operation = 2,
        Brace = 3,
        IdQuote = 4,
        SingleQuote = 5,
        Quote = 6,
        TripleQuote = 7,
        CommentStart = 8,
        MultilineCommentStart = 9,
        BinPrefix = 10,
        HexPrefix = 11,
        Spacing = 12,
        Unknown = 13,
        EndOfFile = 14,
    };

    static const std::unordered_set<char> BraceCharset;
    static const std::unordered_set<char> OperationsCharset;
    static const std::unordered_set<char> SpacingCharset;
    static const std::unordered_set<char> NewlineCharset;
    static const std::unordered_set<std::string> OperationsSets[3];
    static const std::unordered_set<std::string> KeywordSet;
    static const std::unordered_set<char> EscapeCharset;

    static const std::uint64_t IntegersLimits[];

    static CharGroup GetCharGroup(int character);
    static CharGroup GetCharGroup(int character, int lookAhead1, int lookAhead2);

    static bool IsDecChar(int character);
    static bool IsBinChar(int character);
    static bool IsHexChar(int character);

    static LexType GetLeastType(std::uint64_t, LexType initialType);

    static char EscapeToChar(char escapedChar);

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
