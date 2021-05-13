#include "BinOperationNode.h"
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

void BinOperationNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
    visitor.VisitNode(GetLeftOperand(), depth);
    visitor.VisitNode(GetRightOperand(), depth);
}
