#pragma once

#include "IVisitable.h"
#include "Semantics/Symbols.h"

class ISyntaxNode : public IVisitable {
public:
    std::string ToString() const override;
    virtual Lexeme GetLexeme() const = 0;

protected:
    virtual std::string GetName() const = 0;
};

class AbstractNode : public virtual ISyntaxNode {
public:
    AbstractNode(const Lexeme& lexeme);
    AbstractNode(const AbstractNode& src) = delete;
    AbstractNode(AbstractNode&& src) noexcept;

    Lexeme GetLexeme() const override;

private:
    Lexeme myLexeme;
};

class IAnnotatedNode : public virtual ISyntaxNode {
public:
    virtual const ISymbol* GetSymbol() const = 0;
    virtual const ITypeSymbol* GetType() const;
    virtual bool IsAssignable() const;
};
