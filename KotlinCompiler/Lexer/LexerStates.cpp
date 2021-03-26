#include "LexerStates.h"


static LexerState& GetDefaultState(LexerState::CharGroup charGroup, char character) {
    switch (charGroup) {
        case LexerState::CharGroup::Alphabetic:
            return InIdentifierState::Instance();
        case LexerState::CharGroup::Digit:
            return InDigitState::Instance();
        case LexerState::CharGroup::Operation:
            return InOperationState::Instance();
        case LexerState::CharGroup::Brace:
            if (character == '\"') {
                return InStringState::Instance();
            }
            return InSeparatorState::Instance();
        case LexerState::CharGroup::Spacing:
        case LexerState::CharGroup::Unknown:
        default:
            return StartState::Instance();
    }
}

LexerState& StartState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    return GetDefaultState(charGroup, character);
}

LexerState& InDigitState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    LexerState& resultState = GetDefaultState(charGroup, character);

    switch(charGroup) {
        case CharGroup::Digit:
        case CharGroup::Alphabetic:
            return *this;
    }

    return resultState;
}

Lexeme::LexemeType InDigitState::GetLexemeType(char character, bool isStateSwitching) {
    if (isStateSwitching) {
        return Lexeme::LexemeType::Digit;
    }
    
    return Lexeme::LexemeType::Incomplete;
}

LexerState& InIdentifierState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    LexerState& resultState = GetDefaultState(charGroup, character);
    if (charGroup == CharGroup::Digit) {
        return *this;
    }

    return resultState;
}

Lexeme::LexemeType InIdentifierState::GetLexemeType(char character, bool isStateSwitching) {
    if (isStateSwitching) {
        return Lexeme::LexemeType::Identifier;
    }
    return Lexeme::LexemeType::Incomplete;
}

LexerState& InOperationState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    return GetDefaultState(charGroup, character);
}

Lexeme::LexemeType InOperationState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::Operation;
}

LexerState& InSeparatorState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    return GetDefaultState(charGroup, character);
}

Lexeme::LexemeType InSeparatorState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::Separator;
}

LexerState& InStringState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    switch (charGroup) {
        case CharGroup::Alphabetic:
        case CharGroup::Digit:
        case CharGroup::Operation:
        case CharGroup::Spacing:
        case CharGroup::Unknown:
            return *this;
        case CharGroup::Brace:
            if (character == '\"') {
                return StringEndState::Instance();
            }
            return *this;
    }
}

Lexeme::LexemeType InStringState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::Incomplete;
}

LexerState& StringEndState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    return GetDefaultState(charGroup, character);
}

Lexeme::LexemeType StringEndState::GetLexemeType(char character, bool isStateSwitching) {
    return Lexeme::LexemeType::String;
}

LexerState& InCommentState::GetNextState(char character) {
    if (NewlineCharset.count(character)) {
        return StartState::Instance();
    }

    return *this;
}

LexerState& InMultilineCommentState::GetNextState(char character) {
    if (character == '*' || character == '/') {
        return InOperationState::Instance();
    }
    return *this;
}

LexerState& BadState::GetNextState(char character) {
    CharGroup charGroup = GetCharGroup(character);
    return GetDefaultState(charGroup, character);
}

#define SINGLETONE(Type) std::unique_ptr<Type> Type::myInstance(new Type()); \
    Type& Type::Instance() { return *myInstance; }

SINGLETONE(StartState)
SINGLETONE(InDigitState)
SINGLETONE(InIdentifierState)
SINGLETONE(InOperationState)
SINGLETONE(InSeparatorState)
SINGLETONE(InStringState)
SINGLETONE(StringEndState)
SINGLETONE(InCommentState)
SINGLETONE(InMultilineCommentState)
SINGLETONE(BadState)
