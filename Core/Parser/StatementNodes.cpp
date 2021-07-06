#include "StatementNodes.h"
#include "ISyntaxNode.h"
#include "INodeVisitor.h"

EmptyStatement::EmptyStatement(const Lexeme& lexeme, const UnitTypeSymbol* type)
    : UnitTypedNode(lexeme.CopyEmptyOfType(LexemeType::Ignored), type) {}

void EmptyStatement::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

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

void Assignment::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string Assignment::GetName() const {
    return "Assignment :: " + GetOperation();
}

void Assignment::PropagateVisitor(INodeVisitor& visitor) const {
    myAssignable->RunVisitor(visitor);
    myExpression->RunVisitor(visitor);
}

LoopNode::LoopNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type)
    : UnitTypedNode(lexeme, type), myExpression(std::move(expression)), myBody(std::move(body)) {}

const IAnnotatedNode& LoopNode::GetExpression() const {
    return *myExpression;
}

const ISyntaxNode& LoopNode::GetBody() const {
    return *myBody;
}

void LoopNode::PropagateVisitor(INodeVisitor& visitor) const {
    myExpression->RunVisitor(visitor);
    myBody->RunVisitor(visitor);
}

WhileNode::WhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type)
    : LoopNode(lexeme, std::move(expression), std::move(body), type) {}

void WhileNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string WhileNode::GetName() const {
    return "While";
}

DoWhileNode::DoWhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type)
    : LoopNode(lexeme, std::move(expression), std::move(body), type) {}

void DoWhileNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string DoWhileNode::GetName() const {
    return "DoWhile";
}

ForNode::ForNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, Pointer<VariableNode> variable, const UnitTypeSymbol* type)
    : LoopNode(lexeme, std::move(expression), std::move(body), type), myVariable(std::move(variable)) {}

const VariableNode& ForNode::GetVariable() const {
    return *myVariable;
}

void ForNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string ForNode::GetName() const {
    return "For";
}

void ForNode::PropagateVisitor(INodeVisitor& visitor) const {
    myVariable->RunVisitor(visitor);
    LoopNode::PropagateVisitor(visitor);
}
