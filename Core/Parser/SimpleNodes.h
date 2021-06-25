#pragma once
#include "ISyntaxNode.h"
#include "ParserUtils.h"

class LexemeNode : public AbstractNode {
public:
    LexemeNode(const Lexeme& lexeme);

    Lexeme GetLexeme() const;
protected:
    Lexeme myLexeme;
};

class AbstractTypedNode : public LexemeNode, public IAnnotatedNode {
public:
    AbstractTypedNode(const Lexeme& lexeme, const ISymbol* symbol);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    mutable const ISymbol* mySymbol;
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
protected:
    std::string GetName() const override;
};

class BreakNode : public LexemeNode {
public:
    explicit BreakNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class ContinueNode : public LexemeNode {
public:
    explicit ContinueNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class ReturnNode : public LexemeNode {
public:
    explicit ReturnNode(const Lexeme& lexeme);

    const IAnnotatedNode* GetExpression() const;
    void SetExpression(Pointer<IAnnotatedNode> expression);
    bool HasExpression() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
};

// TODO : remove it from node hierarchy
class ErrorNode : public LexemeNode, public virtual IAnnotatedNode {
public:
    ErrorNode(const Lexeme& lexeme, const UnresolvedSymbol* type, const std::string& error = "Unexpected lexeme");

    const ISymbol* GetSymbol() const override;

protected:
    std::string GetName() const override;

private:
    std::string myError;
    const UnresolvedSymbol* myType;
};
