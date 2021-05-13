#pragma once
#include "ISyntaxNode.h"
#include "../Lexer/Lexeme.h"

#include <memory>

class BinOperationNode : public ISyntaxNode {
public:
    BinOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode>&& left, std::unique_ptr<ISyntaxNode>&& right);

    Lexeme GetOperation() const;

    ISyntaxNode& GetLeftOperand() const;
    ISyntaxNode& GetRightOperand() const;

protected:
    std::string GetName() const override {
        return "Bin op :: " + myOperation.GetText();
    }

    void AcceptVisitor(NodeVisitor& visitor, int depth) override;
private:
    std::unique_ptr <ISyntaxNode> myLeftOperand;
    std::unique_ptr <ISyntaxNode> myRightOperand;
    Lexeme myOperation;
};
