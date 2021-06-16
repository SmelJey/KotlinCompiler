#pragma once
#include "DeclarationNodes.h"
#include "ExpressionNodes.h"

class EmptyStatement : public ISyntaxNode {
public:
    EmptyStatement() = default;

protected:
    std::string GetName() const override;
};

class Assignment : public ILexemeNode {
public:
    explicit Assignment(const Lexeme& lexeme);

    std::string GetOperation() const;

    const ISyntaxNode& GetAssignable() const;
    void SetAssignable(Pointer<ISyntaxNode> assignable);

    const ISyntaxNode& GetExpression() const;
    void SetExpression(Pointer<ISyntaxNode> expression);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ISyntaxNode> myAssignable;
    Pointer<ISyntaxNode> myExpression;
};

class ILoopNode : public ISyntaxNode {
public:
    ILoopNode() = default;

    const ISyntaxNode& GetExpression() const;
    void SetExpression(Pointer<ISyntaxNode> expression);

    const ISyntaxNode& GetBody() const;
    void SetBody(Pointer<ISyntaxNode> body);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ISyntaxNode> myExpression;
    Pointer<ISyntaxNode> myBody;
};

class WhileNode : public ILoopNode {
public:
    WhileNode() = default;

protected:
    std::string GetName() const override;
};

class DoWhileNode : public ILoopNode {
public:
    DoWhileNode() = default;

protected:
    std::string GetName() const override;
};

class ForNode : public ILoopNode {
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
