#pragma once

#include "Lexeme.h"


class ILexer {
public:
    virtual ~ILexer() = default;
    Lexeme GetLexeme();
    Lexeme NextLexeme();
protected:
    ILexer();

    Lexeme myCurrentLexeme;
private:
    virtual Lexeme GetLexemeFromInput() = 0;
};
