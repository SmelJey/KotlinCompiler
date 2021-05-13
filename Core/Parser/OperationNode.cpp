#include "OperationNode.h"
#include "ISyntaxNode.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode>&& left,
    std::unique_ptr<ISyntaxNode>&& right) : myLeftOperand(std::move(left)), myRightOperand(std::move(right)), myOperation(operation) {}

Lexeme BinOperationNode::GetOperation() const {
    return myOperation;
}

ISyntaxNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

ISyntaxNode& BinOperationNode::GetRightOperand() const {
    return *myRightOperand;
}

std::string BinOperationNode::GetName() const {
    return "Bin op :: " + myOperation.GetText();
}

void BinOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
    visitor.VisitNode(GetLeftOperand(), depth);
    visitor.VisitNode(GetRightOperand(), depth);
}

UnaryOperationNode::UnaryOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode>&& operand) : myOperand(std::move(operand)), myOperation(operation) {}

Lexeme UnaryOperationNode::GetOperation() const {
    return myOperation;
}

ISyntaxNode& UnaryOperationNode::GetOperand() const {
    return *myOperand;
}

std::string UnaryOperationNode::GetName() const {
    return "Unary op :: " + myOperation.GetText();
}

void UnaryOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
    visitor.VisitNode(*myOperand, depth);
}
