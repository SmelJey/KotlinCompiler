#pragma once
#include <unordered_set>

#include "ISyntaxNode.h"
#include "../Lexer/LexerUtils.h"
#include "Semantics/FundamentalType.h"

class SymbolTable;

class ParserUtils {
public:
    static const std::vector<std::unordered_set<LexemeType>> OperationsPriority;

    static const std::unordered_set<LexemeType> PostfixOperations;

    static const std::unordered_set<LexemeType> AssignmentOperations;

    static bool IsDirectlyAssignable(const IAnnotatedNode* expression);
    static bool IsPostfixUnaryExpression(const IAnnotatedNode* expression);

    static const ISymbol* GetArrayBuilder(const AbstractType* innerType, std::vector<const AbstractType*> argsTypes, SymbolTable* symTable);
    static const ArraySymbol* GetGenericArray(const AbstractType* innerType, SymbolTable* symTable);

    static bool IsGenericIdentifier(const IAnnotatedNode* node);
};
