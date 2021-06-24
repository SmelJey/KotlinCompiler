#include "ISyntaxNode.h"

ISyntaxNode::~ISyntaxNode() = default;

std::string ISyntaxNode::ToString() const {
    return GetName();
}

void ISyntaxNode::RunVisitor(NodeVisitor& visitor) const {
    visitor.VisitNode(*this, 0);
}

void ISyntaxNode::InternalAcceptVisitor(NodeVisitor& visitor, int depth) const {
    AcceptVisitor(visitor, depth);
}

void ISyntaxNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {}

AbstractNode::AbstractNode() = default;
AbstractNode::AbstractNode(AbstractNode&& src) noexcept {}

AbstractNode::~AbstractNode() = default;

const ITypeSymbol* ITypedNode::GetType() const {
    return dynamic_cast<const ITypeSymbol*>(GetSymbol());
}

std::ostream& operator<<(std::ostream& os, const AbstractNode& node) {
    return os << node.ToString();
}
