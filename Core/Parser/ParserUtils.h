#pragma once
#include <unordered_set>

#include "ISyntaxNode.h"
#include "../Lexer/Lexeme.h"

class ParserUtils {
public:
    static const std::vector<std::unordered_set<Lexeme::LexemeType>> OperationsPriority;

    static const std::unordered_set<Lexeme::LexemeType> PostfixOperations;

    static const std::unordered_set<Lexeme::LexemeType> AssignmentOperations;


    static bool IsDirectlyAssignable(const ISyntaxNode* expression);
    static bool IsPostfixUnaryExpression(const ISyntaxNode* expression);
};
