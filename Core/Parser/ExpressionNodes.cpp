#include "ExpressionNodes.h"

#include "AbstractNode.h"
#include "StatementNodes.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, Pointer<ITypedNode> left,
                                   Pointer<ITypedNode> right, const ITypeSymbol* type)
    : LexemeNode(operation), myLeftOperand(std::move(left)), myRightOperand(std::move(right)), myType(type) {}

const ITypedNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

const ITypedNode& BinOperationNode::GetRightOperand() const {
    return *myRightOperand;
}

std::string BinOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

const ISymbol* BinOperationNode::GetSymbol() const {
    return myType;
}

std::string BinOperationNode::GetName() const {
    return "Bin op :: " + GetOperation();
}

void BinOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(GetLeftOperand(), depth);
    visitor.VisitNode(GetRightOperand(), depth);
}

AbstractUnaryOperationNode::AbstractUnaryOperationNode(const Lexeme& operation, Pointer<ITypedNode> operand, const ITypeSymbol* type)
    : LexemeNode(operation), myOperand(std::move(operand)), myType(type) {}

const ITypedNode& AbstractUnaryOperationNode::GetOperand() const {
    return *myOperand;
}

std::string AbstractUnaryOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

const ISymbol* AbstractUnaryOperationNode::GetSymbol() const {
    return myType;
}

void AbstractUnaryOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myOperand, depth);
}

UnaryPrefixOperationNode::UnaryPrefixOperationNode(const Lexeme& operation, Pointer<ITypedNode> operand, const ITypeSymbol* type)
    : AbstractUnaryOperationNode(operation, std::move(operand), type) {}

std::string UnaryPrefixOperationNode::GetName() const {
    return "Prefix Op :: " + GetOperation();
}

UnaryPostfixOperationNode::UnaryPostfixOperationNode(const Lexeme& operation, Pointer<ITypedNode> operand, const ITypeSymbol* type)
    : AbstractUnaryOperationNode(operation, std::move(operand), type) {}

std::string UnaryPostfixOperationNode::GetName() const {
    return "Postfix Op :: " + GetOperation();
}

const std::vector<Pointer<ITypedNode>>& CallArgumentsNode::GetArguments() const {
    return myArguments;
}

void CallArgumentsNode::AddArgument(Pointer<ITypedNode> argument) {
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

PostfixCallNode::PostfixCallNode(Pointer<ITypedNode> expression, const ITypeSymbol* type)
    : myExpression(std::move(expression)), myType(type)  {}

const CallArgumentsNode& PostfixCallNode::GetArguments() const {
    return *myArgumentsNode;
}

void PostfixCallNode::SetArguments(Pointer<CallArgumentsNode> arguments) {
    myArgumentsNode = std::move(arguments);
}

const ITypedNode* PostfixCallNode::GetExpression() const {
    return myExpression.get();
}

const ISymbol* PostfixCallNode::GetSymbol() const {
    return myType;
}

void PostfixCallNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myArgumentsNode, depth);
}

IndexSuffixNode::IndexSuffixNode(Pointer<ITypedNode> expression, const ITypeSymbol* type)
    : PostfixCallNode(std::move(expression), type) {}

std::string IndexSuffixNode::GetName() const {
    return "IndexSuffix";
}

CallSuffixNode::CallSuffixNode(Pointer<ITypedNode> expression, const ITypeSymbol* type)
    : PostfixCallNode(std::move(expression), type) {}

std::string CallSuffixNode::GetName() const {
    return "CallSuffix";
}

MemberAccessNode::MemberAccessNode(const Lexeme& lexeme, Pointer<ITypedNode> expression, Pointer<ITypedNode> member)
    : myOperation(lexeme), myExpression(std::move(expression)), myMemberNode(std::move(member)) {}

std::string MemberAccessNode::GetOperation() const {
    return myOperation.GetValue<std::string>();
}

const ITypedNode* MemberAccessNode::GetExpression() const {
    return myExpression.get();
}

const ITypedNode& MemberAccessNode::GetMember() const {
    return *myMemberNode;
}

const ISymbol* MemberAccessNode::GetSymbol() const {
    return GetMember().GetSymbol();
}

std::string MemberAccessNode::GetName() const {
    return "MemberAccess :: " + GetOperation();
}

void MemberAccessNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myMemberNode, depth);
}

IfExpression::IfExpression(const ITypeSymbol* type) : myType(type) {}

const ITypedNode* IfExpression::GetExpression() const {
    return myExpression.get();
}

void IfExpression::SetExpression(Pointer<ITypedNode> expression) {
    myExpression = std::move(expression);
}

const ISyntaxNode* IfExpression::GetIfBody() const {
    return myIfBody.get();
}

void IfExpression::SetIfBody(Pointer<ISyntaxNode> body) {
    myIfBody = std::move(body);
}

bool IfExpression::HasIfBody() const {
    return dynamic_cast<EmptyStatement*>(myIfBody.get()) == nullptr;
}

const ISyntaxNode* IfExpression::GetElseBody() const {
    return myElseBody.get();
}

void IfExpression::SetElseBody(Pointer<ISyntaxNode> body) {
    myElseBody = std::move(body);
}

bool IfExpression::HasElseBody() const {
    return dynamic_cast<EmptyStatement*>(myElseBody.get()) == nullptr;
}

const ISymbol* IfExpression::GetSymbol() const {
    return myType;
}

std::string IfExpression::GetName() const {
    return "If Expr";
}

void IfExpression::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myIfBody, depth);
    visitor.VisitNode(*myElseBody, depth);
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
