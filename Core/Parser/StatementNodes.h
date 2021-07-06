#pragma once
#include "DeclarationNodes.h"

class EmptyStatement : public UnitTypedNode {
public:
    EmptyStatement(const Lexeme& lexeme, const UnitTypeSymbol* type);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
};

class Assignment : public UnitTypedNode {
public:
    Assignment(const Lexeme& lexeme, Pointer<IAnnotatedNode> assignable, Pointer<IAnnotatedNode> expression, const UnitTypeSymbol* type);

    std::string GetOperation() const;

    const IAnnotatedNode& GetAssignable() const;
    const IAnnotatedNode& GetExpression() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myAssignable;
    Pointer<IAnnotatedNode> myExpression;
};

class LoopNode : public UnitTypedNode {
public:
    LoopNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type);

    const IAnnotatedNode& GetExpression() const;
    const ISyntaxNode& GetBody() const;

protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<ISyntaxNode> myBody;
};

class WhileNode : public LoopNode {
public:
    WhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
};

class DoWhileNode : public LoopNode {
public:
    DoWhileNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, const UnitTypeSymbol* type);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
};

class ForNode : public LoopNode {
public:
    ForNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<ISyntaxNode> body, Pointer<VariableNode> variable, const UnitTypeSymbol* type);

    const VariableNode& GetVariable() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<VariableNode> myVariable;
};
