#pragma once
#include <memory>

#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexeme.h"

class BinOperationNode : public ILexemeNode {
public:
    BinOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> left, Pointer<ISyntaxNode> right);

    const ISyntaxNode& GetLeftOperand() const;
    const ISyntaxNode& GetRightOperand() const;

    std::string GetOperation() const;

protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;
private:
    Pointer<ISyntaxNode> myLeftOperand;
    Pointer<ISyntaxNode> myRightOperand;
};

class IUnaryOperationNode : public ILexemeNode {
public:
    IUnaryOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> operand);

    const ISyntaxNode& GetOperand() const;
    std::string GetOperation() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ISyntaxNode> myOperand;
};

class UnaryPrefixOperationNode : public IUnaryOperationNode {
public:
    UnaryPrefixOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> operand);

protected:
    std::string GetName() const override;
};

class UnaryPostfixOperationNode : public IUnaryOperationNode {
public:
    UnaryPostfixOperationNode(const Lexeme& operation, Pointer<ISyntaxNode> operand);

protected:
    std::string GetName() const override;
};

class IUnaryPostfix : public ISyntaxNode {
public:
    IUnaryPostfix() = default;
};

class CallArgumentsNode : public ISyntaxNode {
public:
    CallArgumentsNode() = default;

    const std::vector<Pointer<ISyntaxNode>>& GetArguments() const;
    void AddArgument(Pointer<ISyntaxNode> argument);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ISyntaxNode>> myArguments;
};

class IPostfixCallNode : public IUnaryPostfix {
public:
    explicit IPostfixCallNode(Pointer<ISyntaxNode> expression);

    const CallArgumentsNode& GetArguments() const;
    void SetArguments(Pointer<CallArgumentsNode> arguments);

    const ISyntaxNode* GetExpression() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ISyntaxNode> myExpression;
    Pointer<CallArgumentsNode> myArgumentsNode;
};

class IndexSuffixNode : public IPostfixCallNode {
public:
    explicit IndexSuffixNode(Pointer<ISyntaxNode> expression);
protected:
    std::string GetName() const override;
};

class CallSuffixNode : public IPostfixCallNode {
public:
    explicit CallSuffixNode(Pointer<ISyntaxNode> expression);
protected:
    std::string GetName() const override;
};

class MemberAccessNode : public IUnaryPostfix {
public:
    MemberAccessNode(const Lexeme& lexeme, Pointer<ISyntaxNode> expression);

    std::string GetOperation() const;

    const ISyntaxNode* GetExpression() const;

    const ISyntaxNode& GetMember() const;
    void SetMember(Pointer<ISyntaxNode> member);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Lexeme myOperation;
    Pointer<ISyntaxNode> myExpression;
    Pointer<ISyntaxNode> myMemberNode;
};

class IfExpression : public ISyntaxNode {
public:
    IfExpression() = default;

    const ISyntaxNode* GetExpression() const;
    void SetExpression(Pointer<ISyntaxNode> expression);

    const ISyntaxNode* GetIfBody() const;
    void SetIfBody(Pointer<ISyntaxNode> body);

    const ISyntaxNode* GetElseBody() const;
    void SetElseBody(Pointer<ISyntaxNode> body);
    bool HasElseBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ISyntaxNode> myExpression;
    Pointer<ISyntaxNode> myIfBody;
    Pointer<ISyntaxNode> myElseBody;
};

class BlockNode : public ISyntaxNode {
public:
    BlockNode() = default;

    const std::vector<Pointer<ISyntaxNode>>& GetStatements() const;
    void AddStatement(Pointer<ISyntaxNode> statement);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ISyntaxNode>> myStatements;
};
