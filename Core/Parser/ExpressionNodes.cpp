#include "ExpressionNodes.h"

#include "ISyntaxNode.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> left,
                                   Pointer<ISyntaxNode> right)
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

IUnaryOperationNode::IUnaryOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> operand)
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

UnaryPrefixOperationNode::UnaryPrefixOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> operand) : IUnaryOperationNode(operation, std::move(operand)) {}

std::string UnaryPrefixOperationNode::GetName() const {
    return "Prefix Op :: " + GetOperation();
}

UnaryPostfixOperationNode::UnaryPostfixOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> operand) : IUnaryOperationNode(operation, std::move(operand)) {}

std::string UnaryPostfixOperationNode::GetName() const {
    return "Postfix Op :: " + GetOperation();
}

const std::vector<Pointer<ISyntaxNode>>& CallArgumentsNode::GetArguments() const {
    return myArguments;
}

void CallArgumentsNode::AddArgument(Pointer<ISyntaxNode> argument) {
    myArguments.push_back(std::move(argument));
}

std::string CallArgumentsNode::GetName() const {
    return "Args";
}

void CallArgumentsNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    for (auto& arg : myArguments) {
        visitor.VisitNode(*arg, depth);
    }
}

IPostfixCallNode::IPostfixCallNode(Pointer<ISyntaxNode> expression) : myExpression(std::move(expression)) {}

const CallArgumentsNode& IPostfixCallNode::GetArguments() const {
    return *myArgumentsNode;
}

void IPostfixCallNode::SetArguments(Pointer<CallArgumentsNode> arguments) {
    myArgumentsNode = std::move(arguments);
}

const ISyntaxNode* IPostfixCallNode::GetExpression() const {
    return myExpression.get();
}

void IPostfixCallNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myArgumentsNode, depth);
}

IndexSuffixNode::IndexSuffixNode(Pointer<ISyntaxNode> expression) : IPostfixCallNode(std::move(expression)) {}

std::string IndexSuffixNode::GetName() const {
    return "IndexSuffix";
}

CallSuffixNode::CallSuffixNode(Pointer<ISyntaxNode> expression) : IPostfixCallNode(std::move(expression)) {}

std::string CallSuffixNode::GetName() const {
    return "CallSuffix";
}

MemberAccessNode::MemberAccessNode(const Lexeme& lexeme, Pointer<ISyntaxNode> expression) : myOperation(lexeme), myExpression(std::move(expression)) {}

std::string MemberAccessNode::GetOperation() const {
    return myOperation.GetValue<std::string>();
}

const ISyntaxNode* MemberAccessNode::GetExpression() const {
    return myExpression.get();
}

const ISyntaxNode& MemberAccessNode::GetMember() const {
    return *myMemberNode;
}

void MemberAccessNode::SetMember(Pointer<ISyntaxNode> member) {
    myMemberNode = std::move(member);
}

std::string MemberAccessNode::GetName() const {
    return "MemberAccess :: " + GetOperation();
}

void MemberAccessNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myMemberNode, depth);
}

const ISyntaxNode* IfExpression::GetExpression() const {
    return myExpression.get();
}

void IfExpression::SetExpression(Pointer<ISyntaxNode> expression) {
    myExpression = std::move(expression);
}

const ISyntaxNode* IfExpression::GetIfBody() const {
    return myIfBody.get();
}

void IfExpression::SetIfBody(Pointer<ISyntaxNode> body) {
    myIfBody = std::move(body);
}

const ISyntaxNode* IfExpression::GetElseBody() const {
    return myElseBody.get();
}

void IfExpression::SetElseBody(Pointer<ISyntaxNode> body) {
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

const std::vector<Pointer<ISyntaxNode>>& BlockNode::GetStatements() const {
    return myStatements;
}

void BlockNode::AddStatement(Pointer<ISyntaxNode> statement) {
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
