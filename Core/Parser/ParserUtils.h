#pragma once
#include <unordered_set>

#include "ISyntaxNode.h"
#include "../Lexer/LexerUtils.h"
#include "Semantics/SymbolTable.h"

class ParserUtils {
public:
    static const std::vector<std::unordered_set<LexemeType>> OperationsPriority;

    static const std::unordered_set<LexemeType> PostfixOperations;

    static const std::unordered_set<LexemeType> AssignmentOperations;

    static bool IsDirectlyAssignable(const ISyntaxNode* expression);
    static bool IsPostfixUnaryExpression(const ISyntaxNode* expression);
};
