#include "catch.hpp"

#include "CompilerTest.h"

TEST_CASE("Lexer Correct Syntax", "[Lexer]") {
    LEXER_TEST("SimpleTest.kt")
    LEXER_TEST("SinglelineComments.kt")
    LEXER_TEST("MultilineComments.kt")
    LEXER_TEST("CharLiterals.kt")
    LEXER_TEST("Strings.kt")
    LEXER_TEST("RawStrings.kt")
    LEXER_TEST("Integers.kt")
    LEXER_TEST("IntegerTypes.kt")
    LEXER_TEST("BinInts.kt")
    LEXER_TEST("HexInts.kt")
    LEXER_TEST("Floats.kt")
    LEXER_TEST("Doubles.kt")
    LEXER_TEST("EscapedIds.kt")
    LEXER_TEST("StringTemplates.kt")
    LEXER_TEST("RawStringTemplates.kt")
    LEXER_TEST("StringNestedTemplates.kt")
}

TEST_CASE("Lexer Incorrect Syntax", "[Lexer]") {
    LEXER_TEST("SimpleErrors.kt")
    LEXER_TEST("CommentsErrors.kt")
    LEXER_TEST("CommentsErrors2.kt")
    LEXER_TEST("IntegersErrors.kt")
    LEXER_TEST("BinIntsErrors.kt")
    LEXER_TEST("HexIntsErrors.kt")
    LEXER_TEST("IntegersOutOfRangeErrors.kt")
    LEXER_TEST("DoubleErrors.kt")
    LEXER_TEST("StringsCharErrors.kt")
    LEXER_TEST("EscapedIdsErrors.kt")
    LEXER_TEST("StringTemplatesErrors1.kt")
    LEXER_TEST("StringTemplatesErrors2.kt")
}

TEST_CASE("Lexer Complex Tests", "[Lexer]") {
    LEXER_TEST("ComplexTests/GenericsExample.kt")
    LEXER_TEST("ComplexTests/ExtensionsExample.kt")
    LEXER_TEST("ComplexTests/InheritanceExample.kt")
    LEXER_TEST("ComplexTests/LambdasExample.kt")
}