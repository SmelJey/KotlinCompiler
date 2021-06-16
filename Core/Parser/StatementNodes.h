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
    void SetAssignable(std::unique_ptr<ISyntaxNode> assignable);

    const ISyntaxNode& GetExpression() const;
    void SetExpression(std::unique_ptr<ISyntaxNode> expression);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myAssignable;
    std::unique_ptr<ISyntaxNode> myExpression;
};

class ILoopNode : public ISyntaxNode {
public:
    ILoopNode() = default;

    const ISyntaxNode& GetExpression() const;
    void SetExpression(std::unique_ptr<ISyntaxNode> expression);

    const ISyntaxNode& GetBody() const;
    void SetBody(std::unique_ptr<ISyntaxNode> body);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myExpression;
    std::unique_ptr<ISyntaxNode> myBody;
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
    void SetVariable(std::unique_ptr<VariableNode> variable);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<VariableNode> myVariable;
};
