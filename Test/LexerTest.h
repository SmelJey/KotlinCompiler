#pragma once

#include "IOTest.h"

template<typename Lexer>
class LexerTest : public IOTest<Lexer> {
public:
    LexerTest(const std::string& filepath) : IOTest<Lexer>(filepath) {}
private:
    std::string NextToken(Lexer& lexer) override {
       /* Lexeme lexeme = lexer.NextLexeme();
        if (lexeme.GetType() == Lexeme::LexemeType::EndOfFile) {
            return "";
        }
        return lexeme.ToString();*/
        return "";
    }
};

#define LEXER_TEST(LexerType, input) \
    IO_TEST(LexerTest<LexerType>, input)