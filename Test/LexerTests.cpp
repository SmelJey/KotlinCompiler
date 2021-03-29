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
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Integers.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "BinInts.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "HexInts.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Floats.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Doubles.kt")
}