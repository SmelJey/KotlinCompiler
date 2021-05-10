#include "catch.hpp"

#include "LexerTest.h"
#include "Lexer/Lexer.h"

#include "Windows.h"
#include <filesystem>

TEST_CASE("Lexer Correct Syntax", "[Lexer]") {
    LEXER_TEST(Lexer, "SimpleTest.kt")
    LEXER_TEST(Lexer, "SinglelineComments.kt")
    LEXER_TEST(Lexer, "MultilineComments.kt")
    LEXER_TEST(Lexer, "CharLiterals.kt")
    LEXER_TEST(Lexer, "Strings.kt")
    LEXER_TEST(Lexer, "RawStrings.kt")
    LEXER_TEST(Lexer, "Integers.kt")
    LEXER_TEST(Lexer, "IntegerTypes.kt")
    LEXER_TEST(Lexer, "BinInts.kt")
    LEXER_TEST(Lexer, "HexInts.kt")
    LEXER_TEST(Lexer, "Floats.kt")
    LEXER_TEST(Lexer, "Doubles.kt")
    LEXER_TEST(Lexer, "EscapedIds.kt")
    LEXER_TEST(Lexer, "StringTemplates.kt")
    LEXER_TEST(Lexer, "RawStringTemplates.kt")
    LEXER_TEST(Lexer, "StringNestedTemplates.kt")
}

TEST_CASE("Lexer Incorrect Syntax", "[Lexer]") {
    LEXER_TEST(Lexer, "SimpleErrors.kt")
    LEXER_TEST(Lexer, "CommentsErrors.kt")
    LEXER_TEST(Lexer, "CommentsErrors2.kt")
    LEXER_TEST(Lexer, "IntegersErrors.kt")
    LEXER_TEST(Lexer, "BinIntsErrors.kt")
    LEXER_TEST(Lexer, "HexIntsErrors.kt")
    LEXER_TEST(Lexer, "IntegersOutOfRangeErrors.kt")
    LEXER_TEST(Lexer, "DoubleErrors.kt")
    LEXER_TEST(Lexer, "StringsCharErrors.kt")
    LEXER_TEST(Lexer, "EscapedIdsErrors.kt")
    LEXER_TEST(Lexer, "StringTemplatesErrors1.kt")
    LEXER_TEST(Lexer, "StringTemplatesErrors2.kt")
}

TEST_CASE("Lexer Complex Tests", "[Lexer]") {
    LEXER_TEST(Lexer, "GenericsExample.kt")
    LEXER_TEST(Lexer, "ExtensionsExample.kt")
    LEXER_TEST(Lexer, "InheritanceExample.kt")
    LEXER_TEST(Lexer, "LambdasExample.kt")
}