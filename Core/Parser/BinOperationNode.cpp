#include "BinOperationNode.h"
Lexeme BinOperationNode::GetOperation() const {
    return myOperation;
}

ISyntaxNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

ISyntaxNode& BinOperationNode::GetRightOperand() const {
    return *myRightOperand;
}
