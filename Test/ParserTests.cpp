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

    PARSER_TEST("Arithmetic/InfixOperator")
    PARSER_TEST("Arithmetic/InRangeOperator")
    PARSER_TEST("Arithmetic/BooleanOperators")
}

TEST_CASE("Parser basic syntax", "[Parser]") {
    PARSER_TEST("BasicSyntax/EmptyFun.kt")
    PARSER_TEST("BasicSyntax/FunParams1.kt")
    PARSER_TEST("BasicSyntax/FunParams2.kt")
    PARSER_TEST("BasicSyntax/FunDefaultParams.kt")

    PARSER_TEST("BasicSyntax/MemberAccess1.kt")
    PARSER_TEST("BasicSyntax/MemberAccess2.kt")
    PARSER_TEST("BasicSyntax/FunctionCall1.kt")
    PARSER_TEST("BasicSyntax/FunctionCall2.kt")
    PARSER_TEST("BasicSyntax/FunctionCall3.kt")
    PARSER_TEST("BasicSyntax/FunctionCall4.kt")
    PARSER_TEST("BasicSyntax/IndexOperator1.kt")
    PARSER_TEST("BasicSyntax/IndexOperator2.kt")
    PARSER_TEST("BasicSyntax/IndexOperator3.kt")
    PARSER_TEST("BasicSyntax/PostfixOperators.kt")

    PARSER_TEST("BasicSyntax/ErrorNoFunName.kt")
    PARSER_TEST("BasicSyntax/ErrorNoIndex.kt")
    PARSER_TEST("BasicSyntax/ErrorNoMember.kt")
    PARSER_TEST("BasicSyntax/ErrorNoParamName.kt")
    PARSER_TEST("BasicSyntax/ErrorNoTypeName.kt")
    PARSER_TEST("BasicSyntax/ErrorNoType.kt")
    PARSER_TEST("BasicSyntax/ErrorNoPropName.kt")

    PARSER_TEST("BasicSyntax/Assignment1.kt")
    PARSER_TEST("BasicSyntax/Assignment2.kt")
    PARSER_TEST("BasicSyntax/Assignment3.kt")

    PARSER_TEST("BasicSyntax/ErrorAssignment1.kt")
    PARSER_TEST("BasicSyntax/ErrorAssignment2.kt")
    PARSER_TEST("BasicSyntax/ErrorAssignment3.kt")
    PARSER_TEST("BasicSyntax/ErrorAssignment4.kt")

    PARSER_TEST("BasicSyntax/IfExpr1.kt")
    PARSER_TEST("BasicSyntax/IfExpr2.kt")
    PARSER_TEST("BasicSyntax/IfExpr3.kt")
    PARSER_TEST("BasicSyntax/IfElseExpr1.kt")
    PARSER_TEST("BasicSyntax/IfElseExpr2.kt")
    PARSER_TEST("BasicSyntax/IfElseExpr3.kt")
    PARSER_TEST("BasicSyntax/IfElseExpr4.kt")
    PARSER_TEST("BasicSyntax/IfElseExpr5.kt")
    PARSER_TEST("BasicSyntax/IfElseExpr6.kt")
    PARSER_TEST("BasicSyntax/IfElseAssignment.kt")

    PARSER_TEST("BasicSyntax/ErrorIf1.kt")
    PARSER_TEST("BasicSyntax/ErrorIf2.kt")
    PARSER_TEST("BasicSyntax/ErrorIf3.kt")
    PARSER_TEST("BasicSyntax/ErrorIf4.kt")
    PARSER_TEST("BasicSyntax/ErrorIfElse1.kt")

    PARSER_TEST("BasicSyntax/PropDecl1.kt")
    PARSER_TEST("BasicSyntax/PropDecl2.kt")
    PARSER_TEST("BasicSyntax/PropDecl3.kt")
    PARSER_TEST("BasicSyntax/PropFunDecl.kt")

    PARSER_TEST("BasicSyntax/ForLoop1.kt")
    PARSER_TEST("BasicSyntax/ForLoop2.kt")

    PARSER_TEST("BasicSyntax/ErrorFor1.kt")
    PARSER_TEST("BasicSyntax/ErrorFor2.kt")
    PARSER_TEST("BasicSyntax/ErrorFor3.kt")
    PARSER_TEST("BasicSyntax/ErrorFor4.kt")
    PARSER_TEST("BasicSyntax/ErrorFor5.kt")
    PARSER_TEST("BasicSyntax/ErrorFor6.kt")

    PARSER_TEST("BasicSyntax/WhileLoop1.kt")
    PARSER_TEST("BasicSyntax/WhileLoop2.kt")
    PARSER_TEST("BasicSyntax/WhileLoop3.kt")

    PARSER_TEST("BasicSyntax/ErrorWhile1.kt")
    PARSER_TEST("BasicSyntax/ErrorWhile2.kt")
    PARSER_TEST("BasicSyntax/ErrorWhile3.kt")
    PARSER_TEST("BasicSyntax/ErrorWhile4.kt")

    PARSER_TEST("BasicSyntax/DoWhile1.kt")
    PARSER_TEST("BasicSyntax/DoWhile2.kt")
    PARSER_TEST("BasicSyntax/DoWhile3.kt")

    PARSER_TEST("BasicSyntax/ErrorDoWhile1.kt")
    PARSER_TEST("BasicSyntax/ErrorDoWhile2.kt")
    PARSER_TEST("BasicSyntax/ErrorDoWhile3.kt")
    PARSER_TEST("BasicSyntax/ErrorDoWhile4.kt")
    PARSER_TEST("BasicSyntax/ErrorDoWhile5.kt")
    PARSER_TEST("BasicSyntax/ErrorDoWhile6.kt")

}
