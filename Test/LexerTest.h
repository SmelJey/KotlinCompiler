#pragma once

#include "IOTest.h"
#include "Lexer/Lexeme.h"

const static std::string LexerDirectory = "LexerTests/";

template<typename Lexer>
class LexerTest : public IOTest<Lexer> {
public:
    LexerTest(const std::string& filepath) : IOTest<Lexer>(LexerDirectory + filepath) {}
private:
    std::string NextToken(Lexer& lexer) override {
        Lexeme lexeme = lexer.NextLexeme();
        if (lexeme.GetType() == Lexeme::LexemeType::EndOfFile) {
            return "";
        }
        return lexeme.ToString();
    }
};

#define LEXER_TEST(LexerType, input) \
    IO_TEST(LexerTest<LexerType>, input)