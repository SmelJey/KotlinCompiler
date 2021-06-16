#include "StatementNodes.h"
#include "ISyntaxNode.h"

std::string EmptyStatement::GetName() const {
    return "Empty Statement";
}

Assignment::Assignment(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string Assignment::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

const ISyntaxNode& Assignment::GetAssignable() const {
    return *myAssignable;
}

void Assignment::SetAssignable(Pointer<ISyntaxNode> assignable) {
    myAssignable = std::move(assignable);
}

const ISyntaxNode& Assignment::GetExpression() const {
    return *myExpression;
}

void Assignment::SetExpression(Pointer<ISyntaxNode> expression) {
    myExpression = std::move(expression);
}

std::string Assignment::GetName() const {
    return "Assignment :: " + GetOperation();
}

void Assignment::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myAssignable, depth);
    visitor.VisitNode(*myExpression, depth);
}

const ISyntaxNode& ILoopNode::GetExpression() const {
    return *myExpression;
}

void ILoopNode::SetExpression(Pointer<ISyntaxNode> expression) {
    myExpression = std::move(expression);
}

const ISyntaxNode& ILoopNode::GetBody() const {
    return *myBody;
}

void ILoopNode::SetBody(Pointer<ISyntaxNode> body) {
    myBody = std::move(body);
}

void ILoopNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myBody, depth);
}

std::string WhileNode::GetName() const {
    return "While";
}

std::string DoWhileNode::GetName() const {
    return "DoWhile";
}

const VariableNode& ForNode::GetVariable() const {
    return *myVariable;
}

void ForNode::SetVariable(Pointer<VariableNode> variable) {
    myVariable = std::move(variable);
}

std::string ForNode::GetName() const {
    return "For";
}

void ForNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myVariable, depth);
    ILoopNode::AcceptVisitor(visitor, depth);
}
