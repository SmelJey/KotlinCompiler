#pragma once
#include <functional>

#include "../Lexer/Lexeme.h"
#include "NodeVisitor.h"

class ISyntaxNode {
public:
    virtual ~ISyntaxNode();

    friend class NodeVisitor;

    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node);

    void RunVisitor(NodeVisitor& visitor);
protected:
    virtual void AcceptVisitor(NodeVisitor& visitor, int depth);

    virtual std::string GetName() const = 0;
};
