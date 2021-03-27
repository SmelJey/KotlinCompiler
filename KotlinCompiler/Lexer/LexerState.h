#pragma once

#include "Lexeme.h"

#include <functional>
#include <utility>
#include <unordered_set>

class LexerState {
public:
    enum class CharGroup : int {
        Digit = 0,
        Alphabetic = 1,
        Operation = 2,
        Brace = 3,
        Spacing = 4,
        Unknown = 5
    };

    const static std::unordered_set<char> OperationsCharset;
    const static std::unordered_set<char> BraceCharset;
    const static std::unordered_set<char> SpacingCharset;
    const static std::unordered_set<char> NewlineCharset;

    static std::function<LexerState& ()> DefaultStateTable[];

    static CharGroup GetCharGroup(char character);

    std::pair<LexerState&, Lexeme::LexemeType> ProcessCharacter(char character);
protected:
    LexerState() = default;

    virtual Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching);
    virtual LexerState& GetNextState(char character, CharGroup charGroup); 
};