#include "ExpressionNodes.h"

#include "AbstractNode.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, Pointer<AbstractNode> left,
                                   Pointer<AbstractNode> right)
    : LexemeNode(operation), myLeftOperand(std::move(left)), myRightOperand(std::move(right)) {}

const AbstractNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

const AbstractNode& BinOperationNode::GetRightOperand() const {
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

AbstractUnaryOperationNode::AbstractUnaryOperationNode(const Lexeme& operation, Pointer<AbstractNode> operand)
    : LexemeNode(operation), myOperand(std::move(operand)) {}

const AbstractNode& AbstractUnaryOperationNode::GetOperand() const {
    return *myOperand;
}

std::string AbstractUnaryOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

void AbstractUnaryOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myOperand, depth);
}

UnaryPrefixOperationNode::UnaryPrefixOperationNode(const Lexeme& operation, Pointer<AbstractNode> operand) : AbstractUnaryOperationNode(operation, std::move(operand)) {}

std::string UnaryPrefixOperationNode::GetName() const {
    return "Prefix Op :: " + GetOperation();
}

UnaryPostfixOperationNode::UnaryPostfixOperationNode(const Lexeme& operation, Pointer<AbstractNode> operand) : AbstractUnaryOperationNode(operation, std::move(operand)) {}

std::string UnaryPostfixOperationNode::GetName() const {
    return "Postfix Op :: " + GetOperation();
}

const std::vector<Pointer<AbstractNode>>& CallArgumentsNode::GetArguments() const {
    return myArguments;
}

void CallArgumentsNode::AddArgument(Pointer<AbstractNode> argument) {
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

PostfixCallNode::PostfixCallNode(Pointer<AbstractNode> expression) : myExpression(std::move(expression)) {}

const CallArgumentsNode& PostfixCallNode::GetArguments() const {
    return *myArgumentsNode;
}

void PostfixCallNode::SetArguments(Pointer<CallArgumentsNode> arguments) {
    myArgumentsNode = std::move(arguments);
}

const AbstractNode* PostfixCallNode::GetExpression() const {
    return myExpression.get();
}

void PostfixCallNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myArgumentsNode, depth);
}

IndexSuffixNode::IndexSuffixNode(Pointer<AbstractNode> expression) : PostfixCallNode(std::move(expression)) {}

std::string IndexSuffixNode::GetName() const {
    return "IndexSuffix";
}

CallSuffixNode::CallSuffixNode(Pointer<AbstractNode> expression) : PostfixCallNode(std::move(expression)) {}

std::string CallSuffixNode::GetName() const {
    return "CallSuffix";
}

MemberAccessNode::MemberAccessNode(const Lexeme& lexeme, Pointer<AbstractNode> expression) : myOperation(lexeme), myExpression(std::move(expression)) {}

std::string MemberAccessNode::GetOperation() const {
    return myOperation.GetValue<std::string>();
}

const AbstractNode* MemberAccessNode::GetExpression() const {
    return myExpression.get();
}

const AbstractNode& MemberAccessNode::GetMember() const {
    return *myMemberNode;
}

void MemberAccessNode::SetMember(Pointer<AbstractNode> member) {
    myMemberNode = std::move(member);
}

std::string MemberAccessNode::GetName() const {
    return "MemberAccess :: " + GetOperation();
}

void MemberAccessNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myMemberNode, depth);
}

const AbstractNode* IfExpression::GetExpression() const {
    return myExpression.get();
}

void IfExpression::SetExpression(Pointer<AbstractNode> expression) {
    myExpression = std::move(expression);
}

const AbstractNode* IfExpression::GetIfBody() const {
    return myIfBody.get();
}

void IfExpression::SetIfBody(Pointer<AbstractNode> body) {
    myIfBody = std::move(body);
}

const AbstractNode* IfExpression::GetElseBody() const {
    return myElseBody.get();
}

void IfExpression::SetElseBody(Pointer<AbstractNode> body) {
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

const std::vector<Pointer<AbstractNode>>& BlockNode::GetStatements() const {
    return myStatements;
}

void BlockNode::AddStatement(Pointer<AbstractNode> statement) {
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
