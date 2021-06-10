#pragma once
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

class ILoopNode : public ILexemeNode {
public:
    explicit ILoopNode(const Lexeme& lexeme);

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
    explicit WhileNode(const Lexeme& lexeme);

protected:
    std::string GetName() const override;
};

class DoWhileNode : public ILoopNode {
public:
    explicit DoWhileNode(const Lexeme& lexeme);

protected:
    std::string GetName() const override;
};

class ForNode : public ILoopNode {
public:
    explicit ForNode(const Lexeme& lexeme);

    const ISyntaxNode& GetVariable() const;
    void SetVariable(std::unique_ptr<ISyntaxNode> variable);

    const ISyntaxNode& GetType() const;
    void SetType(std::unique_ptr<ISyntaxNode> typeNode);
    bool HasType() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myVariable;
    std::unique_ptr<ISyntaxNode> myVarType;
};
