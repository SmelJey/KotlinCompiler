#pragma once

#include <unordered_set>

#include "Lexeme.h"

class ILexer {
public:
    enum class CharGroup : int {
        Digit = 0,
        Alphabetic = 1,
        Operation = 2,
        Brace = 3,
        SingleQuote = 4,
        Quote = 5,
        TripleQuote = 6,
        CommentStart = 7,
        MultilineCommentStart = 8,
        BinPrefix = 9,
        HexPrefix = 10,
        Spacing = 11,
        Unknown = 12,
        EndOfFile = 13,
    };

    static const std::unordered_set<char> BraceCharset;
    static const std::unordered_set<char> OperationsCharset;
    static const std::unordered_set<char> SpacingCharset;
    static const std::unordered_set<char> NewlineCharset;
    static const std::unordered_set<std::string> OperationsSets[3];
    static const std::unordered_set<std::string> KeywordSet;

    static CharGroup GetCharGroup(int character);
    static CharGroup GetCharGroup(int character, int lookAhead1, int lookAhead2);

    static bool IsDecChar(int character);
    static bool IsBinChar(int character);
    static bool IsHexChar(int character);

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
