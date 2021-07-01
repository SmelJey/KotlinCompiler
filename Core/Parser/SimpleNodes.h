#pragma once
#include "ISyntaxNode.h"
#include "ParserUtils.h"

class TypeArgumentsNode;

class AbstractTypedNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    AbstractTypedNode(const Lexeme& lexeme, const ISymbol* symbol);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    mutable const ISymbol* mySymbol;
};

class UnitTypedNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    UnitTypedNode(const Lexeme& lexeme, const UnitTypeSymbol* symbol);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;
private:
    const UnitTypeSymbol* myType;
};

class IdentifierNode : public AbstractTypedNode {
public:
    IdentifierNode(const Lexeme& lexeme, const ITypeSymbol* defaultSym, const std::vector<const ISymbol*>& candidates);

    std::string GetIdentifier() const;

    bool TryResolveVariable();
    bool TryResolveType();
    bool TryResolveFunc(const std::vector<const ITypeSymbol*>& arguments);
    void Resolve(const ISymbol* symbol);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

    bool IsAssignable() const override;
protected:
    std::string GetName() const override;

private:
    std::vector<const ISymbol*> myCandidates;
};

class IntegerNode : public AbstractTypedNode {
public:
    IntegerNode(const Lexeme& lexeme, const ISymbol* symbol);
protected:
    std::string GetName() const override;
};

class DoubleNode : public AbstractTypedNode {
public:
    DoubleNode(const Lexeme& lexeme, const ISymbol* symbol);
protected:
    std::string GetName() const override;
};

class BooleanNode : public AbstractTypedNode {
public:
    BooleanNode(const Lexeme& lexeme, const ISymbol* symbol);
protected:
    std::string GetName() const override;
};

class StringNode : public AbstractTypedNode {
public:
    StringNode(const Lexeme& lexeme, const ISymbol* symbol);
protected:
    std::string GetName() const override;
};

class TypeNode : public AbstractTypedNode {
public:
    TypeNode(const Lexeme& lexeme, const ISymbol* symbol);

    const TypeArgumentsNode* GetTypeArgs() const;
    void SetTypeArgs(Pointer<TypeArgumentsNode> args);
    bool HasTypeArgs() const;
protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

    std::string GetName() const override;
private:
    Pointer<TypeArgumentsNode> myTypeArgs;
};

class BreakNode : public UnitTypedNode {
public:
    BreakNode(const Lexeme& lexeme, const UnitTypeSymbol* type);
protected:
    std::string GetName() const override;
};

class ContinueNode : public UnitTypedNode {
public:
    ContinueNode(const Lexeme& lexeme, const UnitTypeSymbol* type);
protected:
    std::string GetName() const override;
};

class ReturnNode : public UnitTypedNode {
public:
    ReturnNode(const Lexeme& lexeme, const UnitTypeSymbol* type);

    const IAnnotatedNode* GetExpression() const;
    void SetExpression(Pointer<IAnnotatedNode> expression);
    bool HasExpression() const;

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
};
