#include "ISyntaxNode.h"

ISyntaxNode::ISyntaxNode() = default;
ISyntaxNode::ISyntaxNode(ISyntaxNode&& src) noexcept : myErrors(std::move(src.myErrors)) {}

ISyntaxNode::~ISyntaxNode() = default;

std::string ISyntaxNode::ToString() const {
    return GetName();
}

void ISyntaxNode::RunVisitor(NodeVisitor& visitor) {
    visitor.VisitNode(*this, 0);
}

void ISyntaxNode::AddError(std::unique_ptr<ISyntaxNode>&& errorNode) {
    myErrors.push_back(std::move(errorNode));
}

void ISyntaxNode::InternalAcceptVisitor(NodeVisitor& visitor, int depth) {
    AcceptVisitor(visitor, depth);

    for (auto& error : myErrors) {
        visitor.VisitNode(*error, depth);
    }
}

void ISyntaxNode::AcceptVisitor(NodeVisitor& visitor, int depth) {
}


std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node) {
    return os << "<" << node.ToString() << ">";
}
