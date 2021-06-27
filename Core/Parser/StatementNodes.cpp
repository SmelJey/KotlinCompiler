#include "StatementNodes.h"
#include "ISyntaxNode.h"
#include "INodeVisitor.h"

EmptyStatement::EmptyStatement(const Lexeme& lexeme, const UnitTypeSymbol* type)
    : UnitTypedNode(lexeme.CopyEmptyOfType(LexemeType::Ignored), type) {}

std::string EmptyStatement::GetName() const {
    return "Empty Statement";
}

Assignment::Assignment(const Lexeme& lexeme, Pointer<IAnnotatedNode> assignable, Pointer<IAnnotatedNode> expression, const UnitTypeSymbol* type)
    : UnitTypedNode(lexeme, type), myAssignable(std::move(assignable)), myExpression(std::move(expression)) {}

std::string Assignment::GetOperation() const {
    return GetLexeme().GetValue<std::string>();
}

const IAnnotatedNode& Assignment::GetAssignable() const {
    return *myAssignable;
}

const IAnnotatedNode& Assignment::GetExpression() const {
    return *myExpression;
}

std::string Assignment::GetName() const {
    return "Assignment :: " + GetOperation();
}

void Assignment::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myAssignable, depth);
    visitor.VisitNode(*myExpression, depth);
}

LoopNode::LoopNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type)
    : UnitTypedNode(lexeme, type), myExpression(std::move(expression)), myBody(std::move(body)) {}

const IAnnotatedNode& LoopNode::GetExpression() const {
    return *myExpression;
}

const ISyntaxNode& LoopNode::GetBody() const {
    return *myBody;
}

void LoopNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myBody, depth);
}

WhileNode::WhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type)
    : LoopNode(lexeme, std::move(expression), std::move(body), type) {}

std::string WhileNode::GetName() const {
    return "While";
}

DoWhileNode::DoWhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type)
    : LoopNode(lexeme, std::move(expression), std::move(body), type) {}

std::string DoWhileNode::GetName() const {
    return "DoWhile";
}

ForNode::ForNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, Pointer<VariableNode> variable, const UnitTypeSymbol* type)
    : LoopNode(lexeme, std::move(expression), std::move(body), type), myVariable(std::move(variable)) {}

const VariableNode& ForNode::GetVariable() const {
    return *myVariable;
}

std::string ForNode::GetName() const {
    return "For";
}

void ForNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myVariable, depth);
    LoopNode::AcceptVisitor(visitor, depth);
}
