#include "catch.hpp"
#include "CompilerTest.h"

TEST_CASE("Parser arithmetic", "[Parser]") {
    PARSER_TEST("Arithmetic/SimpleSum")
    PARSER_TEST("Arithmetic/MultipleSum")
    PARSER_TEST("Arithmetic/SumAndProd")
    PARSER_TEST("Arithmetic/ProdAndSum")
    PARSER_TEST("Arithmetic/SumParens")
    PARSER_TEST("Arithmetic/SumAndProd")
    PARSER_TEST("Arithmetic/ComplexSumProd")

    PARSER_TEST("Arithmetic/SumSubProdDiv")
    PARSER_TEST("Arithmetic/ComplexTest1")

    PARSER_TEST("Arithmetic/UnaryOperation1")
    PARSER_TEST("Arithmetic/UnaryOperation2")
    PARSER_TEST("Arithmetic/DoubleUnary")
    PARSER_TEST("Arithmetic/ComplexUnaryOperation")

    PARSER_TEST("Arithmetic/Identifiers")

    PARSER_TEST("Arithmetic/UnclosedParenError")
    PARSER_TEST("Arithmetic/UnexpectedLexemeError1")
    PARSER_TEST("Arithmetic/UnexpectedLexemeError2")
    PARSER_TEST("Arithmetic/ErrorNumber")
}

TEST_CASE("Parser basic syntax", "[Parser]") {
    PARSER_TEST("BasicSyntax/emptyFun.kt")
    PARSER_TEST("BasicSyntax/funParams1.kt")
    PARSER_TEST("BasicSyntax/funParams2.kt")
    PARSER_TEST("BasicSyntax/funDefaultParams.kt")
}
