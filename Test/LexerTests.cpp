#include "catch.hpp"

#include "LexerTest.h"
#include "Lexer/IncrementalLexer.h"

#include "Windows.h"
#include <filesystem>

TEST_CASE("Lexer Correct Syntax", "[Lexer]") {
    LEXER_TEST(IncrementalLexer<std::ifstream>, "SimpleTest.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "SinglelineComments.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "MultilineComments.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "CharLiterals.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Strings.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "RawStrings.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Integers.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "BinInts.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "HexInts.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Floats.kt")
    LEXER_TEST(IncrementalLexer<std::ifstream>, "Doubles.kt")
}

TEST_CASE("Lexer Incorrect Syntax", "[Lexer]") {
    LEXER_TEST(IncrementalLexer<std::ifstream>, "SimpleErrors.kt");
    LEXER_TEST(IncrementalLexer<std::ifstream>, "CommentsErrors.kt");
    LEXER_TEST(IncrementalLexer<std::ifstream>, "IntegersErrors.kt");
    LEXER_TEST(IncrementalLexer<std::ifstream>, "BinIntsErrors.kt");
    LEXER_TEST(IncrementalLexer<std::ifstream>, "HexIntsErrors.kt");
    LEXER_TEST(IncrementalLexer<std::ifstream>, "DoubleErrors.kt");
    LEXER_TEST(IncrementalLexer<std::ifstream>, "StringsCharErrors.kt");
}