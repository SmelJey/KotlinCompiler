#include "ExpressionNodes.h"

#include "ISyntaxNode.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> left,
                                   std::unique_ptr<ISyntaxNode> right)
    : ILexemeNode(operation), myLeftOperand(std::move(left)), myRightOperand(std::move(right)) {}

ISyntaxNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

ISyntaxNode& BinOperationNode::GetRightOperand() const {
    return *myRightOperand;
}

std::string BinOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

std::string BinOperationNode::GetName() const {
    return "Bin op :: " + GetOperation();
}

void BinOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
    visitor.VisitNode(GetLeftOperand(), depth);
    visitor.VisitNode(GetRightOperand(), depth);
}

UnaryOperationNode::UnaryOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand)
    : ILexemeNode(operation), myOperand(std::move(operand)) {}

ISyntaxNode& UnaryOperationNode::GetOperand() const {
    return *myOperand;
}

std::string UnaryOperationNode::GetOperation() const {
    return myLexeme.GetValue<std::string>();
}

std::string UnaryOperationNode::GetName() const {
    return "Unary op :: " + GetOperation();
}

void UnaryOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
    visitor.VisitNode(*myOperand, depth);
}
