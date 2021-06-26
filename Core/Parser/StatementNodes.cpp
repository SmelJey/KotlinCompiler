#include "StatementNodes.h"
#include "ISyntaxNode.h"
#include "INodeVisitor.h"

EmptyStatement::EmptyStatement(const Lexeme& lexeme, const UnitTypeSymbol* type)
    : AbstractNode(lexeme.CopyEmptyOfType(LexemeType::Ignored)), myType(type) {}

const ISymbol* EmptyStatement::GetSymbol() const {
    return myType;
}

const ITypeSymbol* EmptyStatement::GetType() const {
    return myType;
}

std::string EmptyStatement::GetName() const {
    return "Empty Statement";
}

Assignment::Assignment(const Lexeme& lexeme, Pointer<IAnnotatedNode> assignable, Pointer<IAnnotatedNode> expression)
    : AbstractNode(lexeme), myAssignable(std::move(assignable)), myExpression(std::move(expression)) {}

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

LoopNode::LoopNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body)
    : AbstractNode(lexeme), myExpression(std::move(expression)), myBody(std::move(body)) {}

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

WhileNode::WhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body)
    : LoopNode(lexeme, std::move(expression), std::move(body)){}

std::string WhileNode::GetName() const {
    return "While";
}

DoWhileNode::DoWhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body)
    : LoopNode(lexeme, std::move(expression), std::move(body)) {}

std::string DoWhileNode::GetName() const {
    return "DoWhile";
}

ForNode::ForNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, Pointer<VariableNode> variable)
    : LoopNode(lexeme, std::move(expression), std::move(body)), myVariable(std::move(variable)) {}

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
