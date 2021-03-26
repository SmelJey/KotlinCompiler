#include "ILexer.h"

ILexer::ILexer() : myCurrentLexeme(0, 0, "", Lexeme::LexemeType::Incomplete) {}

Lexeme ILexer::GetLexeme() {
    if (myCurrentLexeme.GetType() == Lexeme::LexemeType::EndOfFile) {
        myCurrentLexeme = NextLexeme();
    }

    return myCurrentLexeme;
}

Lexeme ILexer::NextLexeme() {
    Lexeme lastLexeme = myCurrentLexeme;
    myCurrentLexeme = GetLexemeFromInput();
    return lastLexeme;
}

