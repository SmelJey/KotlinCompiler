#pragma once

#include "LexerState.h"
#include <memory>

class StartState : public LexerState {
public:
    static StartState& Instance();
private:
    static std::unique_ptr<StartState> myInstance;

    StartState() = default;
};

class InNumberState : public LexerState {
public:
    static InNumberState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
    LexerState& GetNextState(char character, CharGroup charGroup) override;
private:
    static std::unique_ptr<InNumberState> myInstance;

    InNumberState() = default;
};

class InIdentifierState : public LexerState {
public:
    static InIdentifierState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
    LexerState& GetNextState(char character, CharGroup charGroup) override;
private:
    static std::unique_ptr<InIdentifierState> myInstance;

    InIdentifierState() = default;
};

class InOperationState : public LexerState {
public:
    static InOperationState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InOperationState> myInstance;

    InOperationState() = default;
};

class InSeparatorState : public LexerState {
public:
    static InSeparatorState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<InSeparatorState> myInstance;

    InSeparatorState() = default;
};

class InStringState : public LexerState {
public:
    static InStringState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
    LexerState& GetNextState(char character, CharGroup charGroup) override;
private:
    static std::unique_ptr<InStringState> myInstance;

    InStringState() = default;
};

class StringEndState : public LexerState {
public:
    static StringEndState& Instance();
protected:
    Lexeme::LexemeType GetLexemeType(char character, bool isStateSwitching) override;
private:
    static std::unique_ptr<StringEndState> myInstance;

    StringEndState() = default;
};

class InCommentState : public LexerState {
public:
    static InCommentState& Instance();
protected:
    LexerState& GetNextState(char character, CharGroup charGroup) override;
private:
    static std::unique_ptr<InCommentState> myInstance;

    InCommentState() = default;
};

class InMultilineCommentState : public LexerState {
public:
    static InMultilineCommentState& Instance();
protected:
    LexerState& GetNextState(char character, CharGroup charGroup) override;
private:
    static std::unique_ptr<InMultilineCommentState> myInstance;

    InMultilineCommentState() = default;
};

class BadState : public LexerState {
public:
    static BadState& Instance();
private:
    static std::unique_ptr<BadState> myInstance;

    BadState() = default;
};
