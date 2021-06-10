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

class IUnaryPostfix : public ISyntaxNode {};

class IPostfixCallNode : public IUnaryPostfix {
public:
    explicit IPostfixCallNode(std::unique_ptr<ISyntaxNode> expression);

    const std::vector<std::unique_ptr<ISyntaxNode>>& GetArguments() const;
    void SetArguments(std::vector<std::unique_ptr<ISyntaxNode>> arguments);

    const ISyntaxNode* GetExpression() const;

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

class MemberAccessNode : public IUnaryPostfix {
public:
    MemberAccessNode(std::unique_ptr<ISyntaxNode> expression);

    const ISyntaxNode* GetExpression() const;

    const ISyntaxNode& GetMember() const;
    void SetMember(std::unique_ptr<ISyntaxNode> member);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myExpression;
    std::unique_ptr<ISyntaxNode> myMemberNode;
};

class BlockNode : public ISyntaxNode {
public:
    BlockNode() = default;

    const std::vector<std::unique_ptr<ISyntaxNode>>& GetStatements() const;
    void AddStatement(std::unique_ptr<ISyntaxNode> statement);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    // TODO: create statement node
    std::vector<std::unique_ptr<ISyntaxNode>> myStatements;
};
