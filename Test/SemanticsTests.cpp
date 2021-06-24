#include "catch.hpp"
#include "CompilerTest.h"


TEST_CASE("Semantics Declarations", "[Semantics]") {
    RunTests<ParserSemanticTest>("Declarations/");
}

TEST_CASE("Semantics Operations", "[Semantics]") {
    RunTests<ParserSemanticTest>("Operations/");
}
