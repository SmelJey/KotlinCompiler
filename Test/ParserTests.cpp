#include "catch.hpp"

#include "CompilerTest.h"

TEST_CASE("Parser arithmetic", "[Parser]") {
    PARSER_TEST("Arithmetic/SimpleSum")
    PARSER_TEST("Arithmetic/MultipleSum")
    PARSER_TEST("Arithmetic/SumAndProd")
    PARSER_TEST("Arithmetic/ProdAndSum")
}