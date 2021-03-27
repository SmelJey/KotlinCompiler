#include "LexerStates.h"
#include <functional>

LexerState& InNumberState::GetNextState(char character, CharGroup charGroup) {
    if (charGroup == CharGroup::Digit || charGroup == CharGroup::Alphabetic) {
        return *this;
    }

    return LexerState::GetNextState(character, charGroup);
}

Lexeme::LexemeType InNumberState::GetLexemeType(char character, bool isStateSwitching) {
    if (isStateSwitching) {
        return Lexeme::LexemeType::Number;
    }
    
    return Lexeme::LexemeType::Incomplete;
}

LexerState& InIdentifierState::GetNextState(char character, CharGroup charGroup) {
    if (charGroup == CharGroup::Digit) {
        return *this;
    }

    return LexerState::GetNextState(character, charGroup);
}

Lexeme::LexemeType InIdentifierState::GetLexemeType(char character, bool isStateSwitching) {
    if (isStateSwitching) {
        return Lexeme::LexemeType::Identifier;
    }
    return Lexeme::LexemeType::Incomplete;
}

Lexeme::LexemeType InOperationState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::Operation;
}

Lexeme::LexemeType InSeparatorState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::Separator;
}

LexerState& InStringState::GetNextState(char character, CharGroup charGroup) {
    if (character == '\"') {
        return StringEndState::Instance();
    }
    return *this;
}

Lexeme::LexemeType InStringState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::Incomplete;
}

Lexeme::LexemeType StringEndState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::String;
}

LexerState& InCommentState::GetNextState(char character, CharGroup charGroup) {
    if (NewlineCharset.count(character)) {
        return StartState::Instance();
    }

    return *this;
}

LexerState& InMultilineCommentState::GetNextState(char character, CharGroup charGroup) {
    if (character == '*' || character == '/') {
        return InOperationState::Instance();
    }
    return *this;
}

#define SINGLETONE(Type) \
    std::unique_ptr<Type> Type::myInstance(new Type()); \
    Type& Type::Instance() { return *myInstance; }

SINGLETONE(StartState)
SINGLETONE(InNumberState)
SINGLETONE(InIdentifierState)
SINGLETONE(InOperationState)
SINGLETONE(InSeparatorState)
SINGLETONE(InStringState)
SINGLETONE(StringEndState)
SINGLETONE(InCommentState)
SINGLETONE(InMultilineCommentState)
SINGLETONE(BadState)
