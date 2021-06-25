#pragma once

#include "IVisitable.h"
#include "Semantics/Symbols.h"

class ISyntaxNode : public IVisitable {
public:
    std::string ToString() const override;

protected:
    virtual std::string GetName() const = 0;
};

class AbstractNode : public virtual ISyntaxNode {
public:
    AbstractNode();
    AbstractNode(const AbstractNode& src) = delete;
    AbstractNode(AbstractNode&& src) noexcept;
};

class IAnnotatedNode : public virtual ISyntaxNode {
public:
    virtual const ISymbol* GetSymbol() const = 0;
    virtual const ITypeSymbol* GetType() const;
    virtual bool IsAssignable() const;
};
