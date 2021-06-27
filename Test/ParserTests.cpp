#include <filesystem>

#include "catch.hpp"
#include "CompilerTest.h"

TEST_CASE("Parser Arithmetic", "[Parser]") {
    RunTests<ParserTest>("Arithmetic/");
}

TEST_CASE("Parser Basic Syntax", "[Parser]") {
    RunTests<ParserTest>("BasicSyntax/");
}

TEST_CASE("Parser Assignments", "[Parser]") {
    RunTests<ParserTest>("Assignments/");
}

TEST_CASE("Parser Classes", "[Parser]") {
    RunTests<ParserTest>("Classes/");
}

TEST_CASE("Parser IfElse", "[Parser]") {
    RunTests<ParserTest>("IfElse/");
}

TEST_CASE("Parser Jump", "[Parser]") {
    RunTests<ParserTest>("JumpExpressions/");
}

TEST_CASE("Parser Loops", "[Parser]") {
    RunTests<ParserTest>("Loops/");
}

TEST_CASE("Parser Complex Tests", "[Parser]") {
    RunTests<ParserTest>("Complex/");
}
