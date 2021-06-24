#include "ISyntaxNode.h"

std::string ISyntaxNode::ToString() const {
    return GetName();
}

AbstractNode::AbstractNode() = default;
AbstractNode::AbstractNode(AbstractNode&& src) noexcept {}

AbstractNode::~AbstractNode() = default;

const ITypeSymbol* ITypedNode::GetType() const {
    return dynamic_cast<const ITypeSymbol*>(GetSymbol());
}

std::ostream& operator<<(std::ostream& os, const AbstractNode& node) {
    return os << node.ToString();
}
