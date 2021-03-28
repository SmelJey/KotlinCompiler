#include "catch.hpp"

#include "LexerTest.h"
#include "Lexer/IncrementalLexer.h"

#include "Windows.h"
#include <filesystem>

TEST_CASE("Lexer Tests", "[Lexer]") {
    LEXER_TEST(IncrementalLexer<std::ifstream>, "SimpleTest.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "SinglelineComments.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "MultilineComments.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Strings.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "RawStrings.kt")
    //Lexeme test(0, 0, "", Lexeme::LexemeType::EndOfFile);
    //StateLexer<std::ifstream> stateLexer("TestSamples/SimpleTest.kt");
    //LEXER_TEST(StateLexer<std::ifstream>, "SimpleTest.kt");
}