#pragma once
#include "DeclarationNodes.h"

class EmptyStatement : public AbstractNode, public virtual IAnnotatedNode {
public:
    EmptyStatement(const Lexeme& lexeme, const UnitTypeSymbol* type);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    std::string GetName() const override;

private:
    const UnitTypeSymbol* myType;
};

class Assignment : public AbstractNode {
public:
    Assignment(const Lexeme& lexeme, Pointer<IAnnotatedNode> assignable, Pointer<IAnnotatedNode> expression);

    std::string GetOperation() const;

    const IAnnotatedNode& GetAssignable() const;
    const IAnnotatedNode& GetExpression() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myAssignable;
    Pointer<IAnnotatedNode> myExpression;
};

class LoopNode : public AbstractNode {
public:
    LoopNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body);

    const IAnnotatedNode& GetExpression() const;
    const ISyntaxNode& GetBody() const;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<ISyntaxNode> myBody;
};

class WhileNode : public LoopNode {
public:
    WhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body);

protected:
    std::string GetName() const override;
};

class DoWhileNode : public LoopNode {
public:
    DoWhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body);

protected:
    std::string GetName() const override;
};

class ForNode : public LoopNode {
public:
    ForNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, Pointer<VariableNode> variable);

    const VariableNode& GetVariable() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<VariableNode> myVariable;
};
