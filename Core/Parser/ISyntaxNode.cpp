#include "ISyntaxNode.h"

ISyntaxNode::~ISyntaxNode() = default;

std::string ISyntaxNode::ToString() const {
    return GetName();
}

std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node) {
    return os << node.GetName();
}
