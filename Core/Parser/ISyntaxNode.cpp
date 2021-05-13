#include "ISyntaxNode.h"

ISyntaxNode::~ISyntaxNode() = default;

std::string ISyntaxNode::ToString() const {
    return GetName();
}

void ISyntaxNode::RunVisitor(NodeVisitor& visitor) {
    visitor.VisitNode(*this, 0);
}

void ISyntaxNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
}


std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node) {
    return os << "<" << node.ToString() << ">";
}
