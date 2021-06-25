#include "StatementNodes.h"
#include "ISyntaxNode.h"
#include "NodeVisitor.h"

EmptyStatement::EmptyStatement(const UnitTypeSymbol* type) : myType(type) {}

const ISymbol* EmptyStatement::GetSymbol() const {
    return myType;
}

const ITypeSymbol* EmptyStatement::GetType() const {
    return myType;
}

std::string EmptyStatement::GetName() const {
    return "Empty Statement";
}

Assignment::Assignment(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string Assignment::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

const IAnnotatedNode& Assignment::GetAssignable() const {
    return *myAssignable;
}

void Assignment::SetAssignable(Pointer<IAnnotatedNode> assignable) {
    myAssignable = std::move(assignable);
}

const IAnnotatedNode& Assignment::GetExpression() const {
    return *myExpression;
}

void Assignment::SetExpression(Pointer<IAnnotatedNode> expression) {
    myExpression = std::move(expression);
}

std::string Assignment::GetName() const {
    return "Assignment :: " + GetOperation();
}

void Assignment::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myAssignable, depth);
    visitor.VisitNode(*myExpression, depth);
}

const IAnnotatedNode& LoopNode::GetExpression() const {
    return *myExpression;
}

void LoopNode::SetExpression(Pointer<IAnnotatedNode> expression) {
    myExpression = std::move(expression);
}

const ISyntaxNode& LoopNode::GetBody() const {
    return *myBody;
}

void LoopNode::SetBody(Pointer<ISyntaxNode> body) {
    myBody = std::move(body);
}

void LoopNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
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
    LoopNode::AcceptVisitor(visitor, depth);
}
