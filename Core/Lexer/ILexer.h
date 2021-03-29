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
        Quote = 4,
        TripleQuote = 5,
        CommentStart = 6,
        MultilineCommentStart = 7,
        Spacing = 8,
        Unknown = 9,
        EndOfFile = 10,
    };

    static const std::unordered_set<char> BraceCharset;
    static const std::unordered_set<char> OperationsCharset;
    static const std::unordered_set<char> SpacingCharset;
    static const std::unordered_set<char> NewlineCharset;
    static const std::unordered_set<std::string> OperationsSets[3];
    static const std::unordered_set<std::string> KeywordSet;

    static CharGroup GetCharGroup(int character);
    static CharGroup GetCharGroup(int character, int lookAhead1, int lookAhead2);

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
