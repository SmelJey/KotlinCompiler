#pragma once
#include "DeclarationNodes.h"

class EmptyStatement : public AbstractNode, public virtual IAnnotatedNode {
public:
    explicit EmptyStatement(const UnitTypeSymbol* type);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    std::string GetName() const override;

private:
    const UnitTypeSymbol* myType;
};

class Assignment : public LexemeNode {
public:
    explicit Assignment(const Lexeme& lexeme);

    std::string GetOperation() const;

    const IAnnotatedNode& GetAssignable() const;
    void SetAssignable(Pointer<IAnnotatedNode> assignable);

    const IAnnotatedNode& GetExpression() const;
    void SetExpression(Pointer<IAnnotatedNode> expression);

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myAssignable;
    Pointer<IAnnotatedNode> myExpression;
};

class LoopNode : public AbstractNode {
public:
    LoopNode() = default;

    const IAnnotatedNode& GetExpression() const;
    void SetExpression(Pointer<IAnnotatedNode> expression);

    const ISyntaxNode& GetBody() const;
    void SetBody(Pointer<ISyntaxNode> body);

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
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
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<VariableNode> myVariable;
};
