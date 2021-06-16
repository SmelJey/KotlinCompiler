#pragma once
#include <unordered_set>

#include "ISyntaxNode.h"
#include "../Lexer/LexerUtils.h"
#include "../Lexer/Lexeme.h"

template<typename T>
using Pointer = std::unique_ptr<T>;

class ParserUtils {
public:
    static const std::vector<std::unordered_set<LexemeType>> OperationsPriority;

    static const std::unordered_set<LexemeType> PostfixOperations;

    static const std::unordered_set<LexemeType> AssignmentOperations;


    static bool IsDirectlyAssignable(const ISyntaxNode* expression);
    static bool IsPostfixUnaryExpression(const ISyntaxNode* expression);
};
