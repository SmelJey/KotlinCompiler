#include "ExpressionNodes.h"

#include "ISyntaxNode.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> left,
                                   std::unique_ptr<ISyntaxNode> right)
    : ILexemeNode(operation), myLeftOperand(std::move(left)), myRightOperand(std::move(right)) {}

const ISyntaxNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

const ISyntaxNode& BinOperationNode::GetRightOperand() const {
    return *myRightOperand;
}

std::string BinOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

std::string BinOperationNode::GetName() const {
    return "Bin op :: " + GetOperation();
}

void BinOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(GetLeftOperand(), depth);
    visitor.VisitNode(GetRightOperand(), depth);
}

IUnaryOperationNode::IUnaryOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand)
    : ILexemeNode(operation), myOperand(std::move(operand)) {}

const ISyntaxNode& IUnaryOperationNode::GetOperand() const {
    return *myOperand;
}

std::string IUnaryOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

void IUnaryOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myOperand, depth);
}

UnaryPrefixOperationNode::UnaryPrefixOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand) : IUnaryOperationNode(operation, std::move(operand)) {}

std::string UnaryPrefixOperationNode::GetName() const {
    return "Prefix Op :: " + GetOperation();
}

UnaryPostfixOperationNode::UnaryPostfixOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand) : IUnaryOperationNode(operation, std::move(operand)) {}

std::string UnaryPostfixOperationNode::GetName() const {
    return "Postfix Op :: " + GetOperation();
}

IUnaryPostfix::IUnaryPostfix(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

IPostfixCallNode::IPostfixCallNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression) : IUnaryPostfix(lexeme), myExpression(std::move(expression)) {}

const std::vector<std::unique_ptr<ISyntaxNode>>& IPostfixCallNode::GetArguments() const {
    return myArguments;
}

void IPostfixCallNode::SetArguments(std::vector<std::unique_ptr<ISyntaxNode>> arguments) {
    myArguments = std::move(arguments);
}

const ISyntaxNode* IPostfixCallNode::GetExpression() const {
    return myExpression.get();
}

void IPostfixCallNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    for (auto& arg : myArguments) {
        visitor.VisitNode(*arg, depth);
    }
}

IndexSuffixNode::IndexSuffixNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression) : IPostfixCallNode(lexeme, std::move(expression)) {}

std::string IndexSuffixNode::GetName() const {
    return "IndexSuffix";
}

CallSuffixNode::CallSuffixNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression) : IPostfixCallNode(lexeme, std::move(expression)) {}

std::string CallSuffixNode::GetName() const {
    return "CallSuffix";
}

MemberAccessNode::MemberAccessNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression) : IUnaryPostfix(lexeme), myExpression(std::move(expression)) {}

const ISyntaxNode* MemberAccessNode::GetExpression() const {
    return myExpression.get();
}

const ISyntaxNode& MemberAccessNode::GetMember() const {
    return *myMemberNode;
}

void MemberAccessNode::SetMember(std::unique_ptr<ISyntaxNode> member) {
    myMemberNode = std::move(member);
}

std::string MemberAccessNode::GetName() const {
    return "MemberAccess";
}

void MemberAccessNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myMemberNode, depth);
}

IfExpression::IfExpression(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

const ISyntaxNode* IfExpression::GetExpression() const {
    return myExpression.get();
}

void IfExpression::SetExpression(std::unique_ptr<ISyntaxNode> expression) {
    myExpression = std::move(expression);
}

const ISyntaxNode* IfExpression::GetIfBody() const {
    return myIfBody.get();
}

void IfExpression::SetIfBody(std::unique_ptr<ISyntaxNode> body) {
    myIfBody = std::move(body);
}

const ISyntaxNode* IfExpression::GetElseBody() const {
    return myElseBody.get();
}

void IfExpression::SetElseBody(std::unique_ptr<ISyntaxNode> body) {
    myElseBody = std::move(body);
}

bool IfExpression::HasElseBody() const {
    return myElseBody != nullptr;
}

std::string IfExpression::GetName() const {
    return "If Expr";
}

void IfExpression::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myIfBody, depth);
    if (HasElseBody()) {
        visitor.VisitNode(*myElseBody, depth);
    }
}

const std::vector<std::unique_ptr<ISyntaxNode>>& BlockNode::GetStatements() const {
    return myStatements;
}

void BlockNode::AddStatement(std::unique_ptr<ISyntaxNode> statement) {
    myStatements.push_back(std::move(statement));
}

std::string BlockNode::GetName() const {
    return "Block";
}

void BlockNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    for (auto& statement : myStatements) {
        visitor.VisitNode(*statement, depth);
    }
}
