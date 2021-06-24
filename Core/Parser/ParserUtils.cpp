#include "ParserUtils.h"

#include "ExpressionNodes.h"

const std::vector<std::unordered_set<LexemeType>> ParserUtils::OperationsPriority {
    { LexemeType::OpOr},
    { LexemeType::OpAnd},
    { LexemeType::OpEqual, LexemeType::OpStrictEq, LexemeType::OpInequal, LexemeType::OpStrictIneq },
    { LexemeType::OpGreater, LexemeType::OpLess, LexemeType::OpGreaterOrEq, LexemeType::OpLessOrEq },
    { LexemeType::OpIn, LexemeType::OpNotIn },
    // infixFunctionCall
    {},
    { LexemeType::OpDDot },
    { LexemeType::OpAdd, LexemeType::OpSub },
    { LexemeType::OpMult, LexemeType::OpDiv },
};

const std::unordered_set<LexemeType> ParserUtils::PostfixOperations {
    LexemeType::OpDec, LexemeType::OpInc, LexemeType::OpDot, LexemeType::LParen, LexemeType::LSquare
};

const std::unordered_set<LexemeType> ParserUtils::AssignmentOperations {
    LexemeType::OpPlusAssign, LexemeType::OpMinusAssign,
    LexemeType::OpMultAssign, LexemeType::OpDivAssign,
    LexemeType::OpModAssign, LexemeType::OpAssign
};

// postfixUnaryExpression (indexingSuffix | navigationSuffix) | simpleIdentifier | parenthesizedDirectlyAssignableExpression
bool ParserUtils::IsDirectlyAssignable(const ISyntaxNode* expression) {
    const IndexSuffixNode* indexOp = dynamic_cast<const IndexSuffixNode*>(expression);
    if (indexOp != nullptr && IsPostfixUnaryExpression(indexOp->GetExpression())) {
        return true;
    }

    const MemberAccessNode* memberAccess = dynamic_cast<const MemberAccessNode*>(expression);
    if (memberAccess != nullptr && IsPostfixUnaryExpression(memberAccess->GetExpression())) {
        return true;
    }

    const IdentifierNode* identifier = dynamic_cast<const IdentifierNode*>(expression);
    return identifier != nullptr;
}

bool ParserUtils::IsPostfixUnaryExpression(const ISyntaxNode* expression) {
    return dynamic_cast<const AbstractUnaryPostfixNode*>(expression) || dynamic_cast<const IdentifierNode*>(expression);
}
