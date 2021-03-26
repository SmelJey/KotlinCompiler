#pragma once

#include <memory>

#include "LexerState.h"

static LexerState& GetDefaultState(LexerState::CharGroup charGroup, char character);

class StartState : public LexerState {
public:
    static StartState& Instance();
private:
    static std::unique_ptr<StartState> myInstance;

    StartState() = default;

    LexerState& GetNextState(char character) override;
};

class InDigitState : public LexerState {
public:
    static InDigitState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InDigitState> myInstance;

    InDigitState() = default;

    LexerState& GetNextState(char character) override;
};

class InIdentifierState : public LexerState {
public:
    static InIdentifierState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InIdentifierState> myInstance;

    InIdentifierState() = default;

    LexerState& GetNextState(char character) override;
};

class InOperationState : public LexerState {
public:
    static InOperationState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InOperationState> myInstance;

    InOperationState() = default;

    LexerState& GetNextState(char character) override;
};

class InSeparatorState : public LexerState {
public:
    static InSeparatorState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InSeparatorState> myInstance;

    InSeparatorState() = default;

    LexerState& GetNextState(char character) override;
};

class InStringState : public LexerState {
public:
    static InStringState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InStringState> myInstance;

    InStringState() = default;

    LexerState& GetNextState(char character) override;
};

class StringEndState : public LexerState {
public:
    static StringEndState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<StringEndState> myInstance;

    StringEndState() = default;

    LexerState& GetNextState(char character) override;
};

class InCommentState : public LexerState {
public:
    static InCommentState& Instance();
private:
    static std::unique_ptr<InCommentState> myInstance;

    InCommentState() = default;

    LexerState& GetNextState(char character) override;
};

class InMultilineCommentState : public LexerState {
public:
    static InMultilineCommentState& Instance();
private:
    static std::unique_ptr<InMultilineCommentState> myInstance;

    InMultilineCommentState() = default;

    LexerState& GetNextState(char character) override;
};

class BadState : public LexerState {
public:
    static BadState& Instance();
private:
    static std::unique_ptr<BadState> myInstance;

    BadState() = default;

    LexerState& GetNextState(char character) override;
};
