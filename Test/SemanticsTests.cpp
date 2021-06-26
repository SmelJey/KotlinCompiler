#include "catch.hpp"
#include "CompilerTest.h"


TEST_CASE("Semantics Declarations", "[Semantics]") {
    RunTests<ParserSemanticTest>("Declarations/");
}

TEST_CASE("Semantics Arrays", "[Semantics]") {
    RunTests<ParserSemanticTest>("Arrays/");
}

TEST_CASE("Semantics Operations", "[Semantics]") {
    RunTests<ParserSemanticTest>("Operations/");
}

TEST_CASE("Semantics Assignments", "[Semantics]") {
    RunTests<ParserSemanticTest>("Assignments/");
}
