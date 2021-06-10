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

class IUnaryPostfix : public ILexemeNode {
public:
    explicit IUnaryPostfix(const Lexeme& lexeme);
};

class IPostfixCallNode : public IUnaryPostfix {
public:
    explicit IPostfixCallNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression);

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
    explicit IndexSuffixNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression);
protected:
    std::string GetName() const override;
};

class CallSuffixNode : public IPostfixCallNode {
public:
    explicit CallSuffixNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression);
protected:
    std::string GetName() const override;
};

class MemberAccessNode : public IUnaryPostfix {
public:
    MemberAccessNode(const Lexeme& lexeme, std::unique_ptr<ISyntaxNode> expression);

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

class IfExpression : public ILexemeNode {
public:
    explicit IfExpression(const Lexeme& lexeme);

    const ISyntaxNode* GetExpression() const;
    void SetExpression(std::unique_ptr<ISyntaxNode> expression);

    const ISyntaxNode* GetIfBody() const;
    void SetIfBody(std::unique_ptr<ISyntaxNode> body);

    const ISyntaxNode* GetElseBody() const;
    void SetElseBody(std::unique_ptr<ISyntaxNode> body);
    bool HasElseBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myExpression;
    std::unique_ptr<ISyntaxNode> myIfBody;
    std::unique_ptr<ISyntaxNode> myElseBody;
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
    std::vector<std::unique_ptr<ISyntaxNode>> myStatements;
};
