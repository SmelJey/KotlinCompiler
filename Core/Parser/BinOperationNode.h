#pragma once
#include "ISyntaxNode.h"
#include "../Lexer/Lexeme.h"

#include <memory>

class BinOperationNode {
public:
    Lexeme GetOperation() const;

    ISyntaxNode& GetLeftOperand() const;
    ISyntaxNode& GetRightOperand() const;

private:
    std::unique_ptr <ISyntaxNode> myLeftOperand;
    std::unique_ptr <ISyntaxNode> myRightOperand;
    Lexeme myOperation;
};
