#include "StatementNodes.h"
#include "ISyntaxNode.h"

std::string EmptyStatement::GetName() const {
    return "Empty Statement";
}

Assignment::Assignment(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

const ISyntaxNode& Assignment::GetAssignable() const {
    return *myAssignable;
}

void Assignment::SetAssignable(std::unique_ptr<ISyntaxNode> assignable) {
    myAssignable = std::move(assignable);
}

const ISyntaxNode& Assignment::GetExpression() const {
    return *myExpression;
}

void Assignment::SetExpression(std::unique_ptr<ISyntaxNode> expression) {
    myExpression = std::move(expression);
}

std::string Assignment::GetName() const {
    return "Assignment :: =";
}

void Assignment::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myAssignable, depth);
    visitor.VisitNode(*myExpression, depth);
}

ILoopNode::ILoopNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

const ISyntaxNode& ILoopNode::GetExpression() const {
    return *myExpression;
}

void ILoopNode::SetExpression(std::unique_ptr<ISyntaxNode> expression) {
    myExpression = std::move(expression);
}

const ISyntaxNode& ILoopNode::GetBody() const {
    return *myBody;
}

void ILoopNode::SetBody(std::unique_ptr<ISyntaxNode> body) {
    myBody = std::move(body);
}

void ILoopNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myBody, depth);
}

WhileNode::WhileNode(const Lexeme& lexeme) : ILoopNode(lexeme) {}

std::string WhileNode::GetName() const {
    return "While";
}

DoWhileNode::DoWhileNode(const Lexeme& lexeme) : ILoopNode(lexeme) {}

std::string DoWhileNode::GetName() const {
    return "DoWhile";
}

ForNode::ForNode(const Lexeme& lexeme) : ILoopNode(lexeme) {}

const ISyntaxNode& ForNode::GetVariable() const {
    return *myVariable;
}

void ForNode::SetVariable(std::unique_ptr<ISyntaxNode> variable) {
    myVariable = std::move(variable);
}

const ISyntaxNode& ForNode::GetType() const {
    return *myVarType;
}

void ForNode::SetType(std::unique_ptr<ISyntaxNode> typeNode) {
    myVarType = std::move(typeNode);
}

bool ForNode::HasType() const {
    return myVarType != nullptr;
}

std::string ForNode::GetName() const {
    return "For";
}

void ForNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myVariable, depth);
    if (HasType()) {
        visitor.VisitNode(*myVarType, depth);
    }
    ILoopNode::AcceptVisitor(visitor, depth);
}
