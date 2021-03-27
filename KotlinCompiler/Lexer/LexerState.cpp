#include "LexerState.h"

#include "LexerStates.h"

#include <functional>

const std::unordered_set<char> LexerState::BraceCharset { '(', ')', '[', ']', '{', '}', '\"', '\'' };
const std::unordered_set<char> LexerState::OperationsCharset { '+', '-', '*', '/', '=', '>', '<', '.',
        ',', ';', ':', '@', '$', '?', '!', '#', '|', '&', '^', '%' };
const std::unordered_set<char> LexerState::SpacingCharset { ' ', '\t', '\r', '\n' };
const std::unordered_set<char> LexerState::NewlineCharset { '\n', '\r'};

LexerState::CharGroup LexerState::GetCharGroup(char character) {
    if (character >= '0' && character <= '9') {
        return CharGroup::Digit;
    }
    if (character >= 'a' && character <= 'z'
        || character >= 'A' && character <= 'Z' || character == '_') {
        return CharGroup::Alphabetic;
    }

    if (OperationsCharset.count(character)) {
        return CharGroup::Operation;
    }
    if (BraceCharset.count(character)) {
        return CharGroup::Brace;
    }
    if (SpacingCharset.count(character)) {
        return CharGroup::Spacing;
    }
    return CharGroup::Unknown;
}

std::pair<LexerState&, Lexeme::LexemeType> LexerState::ProcessCharacter(char character) {
    LexerState& nextState = GetNextState(character, GetCharGroup(character));

    Lexeme::LexemeType lexemeType = GetLexemeType(character, this != &nextState);

    return std::make_pair(std::ref(nextState), lexemeType);
}

Lexeme::LexemeType LexerState::GetLexemeType(char character, bool isStateSwitching) {
    if (isStateSwitching) {
        return Lexeme::LexemeType::Ignored;
    }
    return Lexeme::LexemeType::Incomplete;
}

std::function<LexerState& ()> LexerState::DefaultStateTable[] {
    InNumberState::Instance, InIdentifierState::Instance, InOperationState::Instance,
    InSeparatorState::Instance, StartState::Instance, StartState::Instance,
};

LexerState& LexerState::GetNextState(char character, CharGroup charGroup) {
    if (character == '\"') {
        return InStringState::Instance();
    }
    return DefaultStateTable[(int)charGroup]();
}
