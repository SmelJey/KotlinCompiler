#include "catch.hpp"
#include "CompilerTest.h"

TEST_CASE("Lexer Basic Syntax", "[Lexer]") {
    RunTests<LexerTest>("Basic/");
}

TEST_CASE("Lexer Numbers", "[Lexer]") {
    RunTests<LexerTest>("Numbers/");
}

TEST_CASE("Lexer String Literals", "[Lexer]") {
    RunTests<LexerTest>("StringLiterals/");
}

TEST_CASE("Lexer Complex Tests", "[Lexer]") {
    RunTests<LexerTest>("Complex/");
}