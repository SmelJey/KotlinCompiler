#pragma once
#include "DeclarationNodes.h"
#include "ExpressionNodes.h"

class EmptyStatement : public AbstractNode {
public:
    EmptyStatement() = default;

protected:
    std::string GetName() const override;
};

class Assignment : public LexemeNode {
public:
    explicit Assignment(const Lexeme& lexeme);

    std::string GetOperation() const;

    const AbstractNode& GetAssignable() const;
    void SetAssignable(Pointer<AbstractNode> assignable);

    const AbstractNode& GetExpression() const;
    void SetExpression(Pointer<AbstractNode> expression);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myAssignable;
    Pointer<AbstractNode> myExpression;
};

class LoopNode : public AbstractNode {
public:
    LoopNode() = default;

    const AbstractNode& GetExpression() const;
    void SetExpression(Pointer<AbstractNode> expression);

    const AbstractNode& GetBody() const;
    void SetBody(Pointer<AbstractNode> body);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myExpression;
    Pointer<AbstractNode> myBody;
};

class WhileNode : public LoopNode {
public:
    WhileNode() = default;

protected:
    std::string GetName() const override;
};

class DoWhileNode : public LoopNode {
public:
    DoWhileNode() = default;

protected:
    std::string GetName() const override;
};

class ForNode : public LoopNode {
public:
    ForNode() = default;

    const VariableNode& GetVariable() const;
    void SetVariable(Pointer<VariableNode> variable);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<VariableNode> myVariable;
};
