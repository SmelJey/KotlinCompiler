#include "LexerState.h"


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
    return CharGroup::Unknown;
}

std::pair<LexerState&, Lexeme::LexemeType> LexerState::ProcessCharacter(char character) {
    Lexeme::LexemeType status = Lexeme::LexemeType::Incomplete;
    LexerState& nextState = GetNextState(character);

    status = GetLexemeType(character, this != &nextState);

    return std::make_pair(std::ref(nextState), status);
}

Lexeme::LexemeType LexerState::GetLexemeType(char character, bool isStateSwitching) {
    if (isStateSwitching) {
        return Lexeme::LexemeType::Ignored;
    }
    return Lexeme::LexemeType::Incomplete;
}
