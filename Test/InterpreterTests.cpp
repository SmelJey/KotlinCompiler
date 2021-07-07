#include "catch.hpp"
#include "InterpreterTest.h"

TEST_CASE("Interpreter Basic Syntax", "[Interpreter]") {
    InterpreterTest::RunTests("BasicSyntax/");
}

TEST_CASE("Interpreter Variables", "[Interpreter]") {
    InterpreterTest::RunTests("Variables/");
}

TEST_CASE("Interpreter If Expressions", "[Interpreter]") {
    InterpreterTest::RunTests("IfExpr/");
}

TEST_CASE("Interpreter Loops", "[Interpreter]") {
    InterpreterTest::RunTests("Loops/");
}