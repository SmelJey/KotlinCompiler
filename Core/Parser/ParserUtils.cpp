#include "ParserUtils.h"

const std::vector<std::unordered_set<Lexeme::LexemeType>> ParserUtils::OperationsPriority {
    { Lexeme::LexemeType::OpOr},
    { Lexeme::LexemeType::OpAnd},
    { Lexeme::LexemeType::OpEqual, Lexeme::LexemeType::OpStrictEq, Lexeme::LexemeType::OpInequal, Lexeme::LexemeType::OpStrictIneq },
    { Lexeme::LexemeType::OpGreater, Lexeme::LexemeType::OpLess, Lexeme::LexemeType::OpGreaterOrEq, Lexeme::LexemeType::OpLessOrEq },
    { Lexeme::LexemeType::OpIn, Lexeme::LexemeType::OpNotIn },
    // infixFunctionCall
    {},
    { Lexeme::LexemeType::OpDDot },
    { Lexeme::LexemeType::OpAdd, Lexeme::LexemeType::OpSub },
    { Lexeme::LexemeType::OpMult, Lexeme::LexemeType::OpDiv },
};

const std::unordered_set<Lexeme::LexemeType> ParserUtils::PostfixOperations {
    Lexeme::LexemeType::OpDec, Lexeme::LexemeType::OpInc, Lexeme::LexemeType::OpDot, Lexeme::LexemeType::LParen, Lexeme::LexemeType::LSquare
};
