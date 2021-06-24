#pragma once
#include "DeclarationNodes.h"

class EmptyStatement : public AbstractNode, public virtual ITypedNode {
public:
    explicit EmptyStatement(const UnitTypeSymbol* type);

    const ISymbol* GetSymbol() const override;

protected:
    std::string GetName() const override;

private:
    const UnitTypeSymbol* myType;
};

class Assignment : public LexemeNode {
public:
    explicit Assignment(const Lexeme& lexeme);

    std::string GetOperation() const;

    const ITypedNode& GetAssignable() const;
    void SetAssignable(Pointer<ITypedNode> assignable);

    const ITypedNode& GetExpression() const;
    void SetExpression(Pointer<ITypedNode> expression);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myAssignable;
    Pointer<ITypedNode> myExpression;
};

class LoopNode : public AbstractNode {
public:
    LoopNode() = default;

    const ITypedNode& GetExpression() const;
    void SetExpression(Pointer<ITypedNode> expression);

    const ISyntaxNode& GetBody() const;
    void SetBody(Pointer<ISyntaxNode> body);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myExpression;
    Pointer<ISyntaxNode> myBody;
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
