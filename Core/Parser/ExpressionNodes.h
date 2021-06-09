#pragma once
#include "ISyntaxNode.h"
#include "../Lexer/Lexeme.h"

#include <memory>

#include "SimpleNodes.h"

class BinOperationNode : public ILexemeNode {
public:
    BinOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> left, std::unique_ptr<ISyntaxNode> right);

    ISyntaxNode& GetLeftOperand() const;
    ISyntaxNode& GetRightOperand() const;

    std::string GetOperation() const;

protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) override;
private:
    std::unique_ptr<ISyntaxNode> myLeftOperand;
    std::unique_ptr<ISyntaxNode> myRightOperand;
};

class UnaryOperationNode : public ILexemeNode {
public:
    UnaryOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand);

    ISyntaxNode& GetOperand() const;

    std::string GetOperation() const;

protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) override;

private:
    std::unique_ptr<ISyntaxNode> myOperand;
};
