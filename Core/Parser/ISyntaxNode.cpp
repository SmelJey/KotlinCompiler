#include "ISyntaxNode.h"

std::string ISyntaxNode::ToString() const {
    return GetName();
}

AbstractNode::AbstractNode() = default;
AbstractNode::AbstractNode(AbstractNode&& src) noexcept {}

const ITypeSymbol* IAnnotatedNode::GetType() const {
    return nullptr;
}

bool IAnnotatedNode::IsAssignable() const {
    return false;
}

std::ostream& operator<<(std::ostream& os, const AbstractNode& node) {
    return os << node.ToString();
}
