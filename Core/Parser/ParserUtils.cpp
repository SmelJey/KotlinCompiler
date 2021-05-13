#include "ParserUtils.h"

const std::vector<std::unordered_set<Lexeme::LexemeType>> ParserUtils::OperationsPriority {
    { Lexeme::LexemeType::OpAdd, Lexeme::LexemeType::OpSub },
    { Lexeme::LexemeType::OpMult, Lexeme::LexemeType::OpDiv },
};
