#pragma once
#include <memory>

#include "AbstractNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexeme.h"

class BinOperationNode : public LexemeNode {
public:
    BinOperationNode(const Lexeme& operation, Pointer<AbstractNode> left, Pointer<AbstractNode> right);

    const AbstractNode& GetLeftOperand() const;
    const AbstractNode& GetRightOperand() const;

    std::string GetOperation() const;

protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;
private:
    Pointer<AbstractNode> myLeftOperand;
    Pointer<AbstractNode> myRightOperand;
};

class AbstractUnaryOperationNode : public LexemeNode {
public:
    AbstractUnaryOperationNode(const Lexeme& operation, Pointer<AbstractNode> operand);

    const AbstractNode& GetOperand() const;
    std::string GetOperation() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myOperand;
};

class UnaryPrefixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPrefixOperationNode(const Lexeme& operation, Pointer<AbstractNode> operand);

protected:
    std::string GetName() const override;
};

class UnaryPostfixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPostfixOperationNode(const Lexeme& operation, Pointer<AbstractNode> operand);

protected:
    std::string GetName() const override;
};

class AbstractUnaryPostfixNode : public AbstractNode {
public:
    AbstractUnaryPostfixNode() = default;
};

class CallArgumentsNode : public AbstractNode {
public:
    CallArgumentsNode() = default;

    const std::vector<Pointer<AbstractNode>>& GetArguments() const;
    void AddArgument(Pointer<AbstractNode> argument);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<AbstractNode>> myArguments;
};

class PostfixCallNode : public AbstractUnaryPostfixNode {
public:
    explicit PostfixCallNode(Pointer<AbstractNode> expression);

    const CallArgumentsNode& GetArguments() const;
    void SetArguments(Pointer<CallArgumentsNode> arguments);

    const AbstractNode* GetExpression() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myExpression;
    Pointer<CallArgumentsNode> myArgumentsNode;
};

class IndexSuffixNode : public PostfixCallNode {
public:
    explicit IndexSuffixNode(Pointer<AbstractNode> expression);
protected:
    std::string GetName() const override;
};

class CallSuffixNode : public PostfixCallNode {
public:
    explicit CallSuffixNode(Pointer<AbstractNode> expression);
protected:
    std::string GetName() const override;
};

class MemberAccessNode : public AbstractUnaryPostfixNode {
public:
    MemberAccessNode(const Lexeme& lexeme, Pointer<AbstractNode> expression);

    std::string GetOperation() const;

    const AbstractNode* GetExpression() const;

    const AbstractNode& GetMember() const;
    void SetMember(Pointer<AbstractNode> member);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Lexeme myOperation;
    Pointer<AbstractNode> myExpression;
    Pointer<AbstractNode> myMemberNode;
};

class IfExpression : public AbstractNode {
public:
    IfExpression() = default;

    const AbstractNode* GetExpression() const;
    void SetExpression(Pointer<AbstractNode> expression);

    const AbstractNode* GetIfBody() const;
    void SetIfBody(Pointer<AbstractNode> body);
    bool HasIfBody() const;

    const AbstractNode* GetElseBody() const;
    void SetElseBody(Pointer<AbstractNode> body);
    bool HasElseBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myExpression;
    Pointer<AbstractNode> myIfBody;
    Pointer<AbstractNode> myElseBody;
};

class BlockNode : public AbstractNode {
public:
    BlockNode() = default;

    const std::vector<Pointer<AbstractNode>>& GetStatements() const;
    void AddStatement(Pointer<AbstractNode> statement);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<AbstractNode>> myStatements;
};
