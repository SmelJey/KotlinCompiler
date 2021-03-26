#pragma once

#include "Lexeme.h"


class ILexer {
public:
    virtual ~ILexer() = default;
    Lexeme GetLexeme();
    Lexeme NextLexeme();
protected:
    ILexer();
private:
    Lexeme myCurrentLexeme;

    virtual Lexeme GetLexemeFromInput() = 0;
};
