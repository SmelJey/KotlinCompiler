#pragma once

#include "../Lexer/Lexeme.h"
#include "NodeVisitor.h"

class ISyntaxNode {
public:
    ISyntaxNode();

    ISyntaxNode(const ISyntaxNode& src) = delete;

    ISyntaxNode(ISyntaxNode&& src) noexcept;

    virtual ~ISyntaxNode();

    friend class NodeVisitor;

    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node);

    void RunVisitor(NodeVisitor& visitor);

    void AddError(std::unique_ptr<ISyntaxNode>&& errorNode);

protected:
    void InternalAcceptVisitor(NodeVisitor& visitor, int depth);

    virtual void AcceptVisitor(NodeVisitor& visitor, int depth);

    virtual std::string GetName() const = 0;

private:
    std::vector<std::unique_ptr<ISyntaxNode>> myErrors;
};
