#pragma once

#include "NodeVisitor.h"
#include "../Lexer/Lexeme.h"

class AbstractNode {
public:
    AbstractNode();

    AbstractNode(const AbstractNode& src) = delete;

    AbstractNode(AbstractNode&& src) noexcept;

    virtual ~AbstractNode();

    friend class NodeVisitor;

    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const AbstractNode& node);

    void RunVisitor(NodeVisitor& visitor) const;

    void AddError(std::unique_ptr<AbstractNode>&& errorNode);

protected:
    void InternalAcceptVisitor(NodeVisitor& visitor, int depth) const;

    virtual void AcceptVisitor(NodeVisitor& visitor, int depth) const;

    virtual std::string GetName() const = 0;

private:
    std::vector<std::unique_ptr<AbstractNode>> myErrors;
};
