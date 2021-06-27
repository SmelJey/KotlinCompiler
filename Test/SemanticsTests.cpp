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

TEST_CASE("Semantics Arithmetic", "[Semantics]") {
    RunTests<ParserSemanticTest>("Arithmetic/");
}

TEST_CASE("Semantics Basic syntax", "[Semantics]") {
    RunTests<ParserSemanticTest>("BasicSyntax/");
}

TEST_CASE("Semantics Assignments", "[Semantics]") {
    RunTests<ParserSemanticTest>("Assignments/");
}

TEST_CASE("Semantics Classes", "[Semantics]") {
    RunTests<ParserSemanticTest>("Classes/");
}

//TEST_CASE("Semantics IfElse", "[Semantics]") {
//    RunTests<ParserSemanticTest>("IfElse/");
//}
//
//TEST_CASE("Semantics Loops", "[Semantics]") {
//    RunTests<ParserSemanticTest>("Loops/");
//}
//
//TEST_CASE("Semantics Jumps", "[Semantics]") {
//    RunTests<ParserSemanticTest>("Jumps/");
//}
