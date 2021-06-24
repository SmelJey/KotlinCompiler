#pragma once

#include "NodeVisitor.h"
#include "../Lexer/Lexeme.h"
#include "Semantics/Symbols.h"

class ISyntaxNode {
public:
    virtual ~ISyntaxNode();
    friend std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node);
    friend class NodeVisitor;

    std::string ToString() const;
    void RunVisitor(NodeVisitor& visitor) const;
    virtual void AddError(Pointer<ISyntaxNode> errorNode) = 0;

protected:
    void InternalAcceptVisitor(NodeVisitor& visitor, int depth) const;
    virtual void AcceptVisitor(NodeVisitor& visitor, int depth) const;

    virtual std::string GetName() const = 0;
    virtual const std::vector<Pointer<ISyntaxNode>>& GetErrors() const = 0;
};

class AbstractNode : public virtual ISyntaxNode {
public:
    AbstractNode();
    AbstractNode(const AbstractNode& src) = delete;
    AbstractNode(AbstractNode&& src) noexcept;

    virtual ~AbstractNode();
    void AddError(Pointer<ISyntaxNode> errorNode) override;

protected:
    const std::vector<Pointer<ISyntaxNode>>& GetErrors() const override;

private:
    std::vector<Pointer<ISyntaxNode>> myErrors;
};

class ISymbolAnnotatedNode : public virtual ISyntaxNode {
public:
    virtual const ISymbol* GetSymbol() const = 0;
};

class ITypedNode : public ISymbolAnnotatedNode {
public:
    virtual const ITypeSymbol* GetType() const;
};
