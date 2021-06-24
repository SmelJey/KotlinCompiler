#pragma once

#include "NodeVisitor.h"
#include "Semantics/Symbols.h"

class ISyntaxNode {
public:
    virtual ~ISyntaxNode();
    friend class NodeVisitor;

    std::string ToString() const;
    void RunVisitor(NodeVisitor& visitor) const;

protected:
    void InternalAcceptVisitor(NodeVisitor& visitor, int depth) const;
    virtual void AcceptVisitor(NodeVisitor& visitor, int depth) const;

    virtual std::string GetName() const = 0;
};

class AbstractNode : public virtual ISyntaxNode {
public:
    AbstractNode();
    AbstractNode(const AbstractNode& src) = delete;
    AbstractNode(AbstractNode&& src) noexcept;

    virtual ~AbstractNode();
};

class ISymbolAnnotatedNode : public virtual ISyntaxNode {
public:
    virtual const ISymbol* GetSymbol() const = 0;
};

class ITypedNode : public ISymbolAnnotatedNode {
public:
    virtual const ITypeSymbol* GetType() const;
};
