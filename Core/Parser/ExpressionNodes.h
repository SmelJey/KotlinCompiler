#pragma once
#include "ISyntaxNode.h"
#include "../Lexer/Lexeme.h"

#include <memory>

#include "SimpleNodes.h"

class BinOperationNode : public ILexemeNode {
public:
    BinOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> left, std::unique_ptr<ISyntaxNode> right);

    const ISyntaxNode& GetLeftOperand() const;
    const ISyntaxNode& GetRightOperand() const;

    std::string GetOperation() const;

protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;
private:
    std::unique_ptr<ISyntaxNode> myLeftOperand;
    std::unique_ptr<ISyntaxNode> myRightOperand;
};

class IUnaryOperationNode : public ILexemeNode {
public:
    IUnaryOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand);

    const ISyntaxNode& GetOperand() const;
    std::string GetOperation() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myOperand;
};

class UnaryPrefixOperationNode : public IUnaryOperationNode {
public:
    UnaryPrefixOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand);

protected:
    std::string GetName() const override;
};

class UnaryPostfixOperationNode : public IUnaryOperationNode {
public:
    UnaryPostfixOperationNode(const Lexeme& operation, std::unique_ptr<ISyntaxNode> operand);

protected:
    std::string GetName() const override;
};

class IPostfixCallNode : public ISyntaxNode {
public:
    explicit IPostfixCallNode(std::unique_ptr<ISyntaxNode> expression);

    const std::vector<std::unique_ptr<ISyntaxNode>>& GetArguments() const;
    void SetArguments(std::vector<std::unique_ptr<ISyntaxNode>> arguments);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myExpression;
    std::vector<std::unique_ptr<ISyntaxNode>> myArguments;
};


class IndexSuffixNode : public IPostfixCallNode {
public:
    explicit IndexSuffixNode(std::unique_ptr<ISyntaxNode> expression);
protected:
    std::string GetName() const override;
};


class CallSuffixNode : public IPostfixCallNode {
public:
    explicit CallSuffixNode(std::unique_ptr<ISyntaxNode> expression);
protected:
    std::string GetName() const override;
};

class MemberAccessNode : public ISyntaxNode {
public:
    MemberAccessNode(std::unique_ptr<ISyntaxNode> expression);

    const ISyntaxNode& GetMember() const;
    void SetMember(std::unique_ptr<ISyntaxNode> member);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myExpression;
    std::unique_ptr<ISyntaxNode> myMemberNode;
};
