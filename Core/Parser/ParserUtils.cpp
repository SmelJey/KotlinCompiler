#include "ParserUtils.h"

#include "ExpressionNodes.h"

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

const std::unordered_set<Lexeme::LexemeType> ParserUtils::AssignmentOperations {
    Lexeme::LexemeType::OpPlusAssign, Lexeme::LexemeType::OpMinusAssign,
    Lexeme::LexemeType::OpMultAssign, Lexeme::LexemeType::OpDivAssign,
    Lexeme::LexemeType::OpModAssign, Lexeme::LexemeType::OpAssign
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
    return dynamic_cast<const IUnaryPostfix*>(expression) || dynamic_cast<const IdentifierNode*>(expression);
}
