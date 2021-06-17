#include "AbstractNode.h"

AbstractNode::AbstractNode() = default;
AbstractNode::AbstractNode(AbstractNode&& src) noexcept : myErrors(std::move(src.myErrors)) {}

AbstractNode::~AbstractNode() = default;

std::string AbstractNode::ToString() const {
    return GetName();
}

void AbstractNode::RunVisitor(NodeVisitor& visitor) const {
    visitor.VisitNode(*this, 0);
}

void AbstractNode::AddError(std::unique_ptr<AbstractNode>&& errorNode) {
    myErrors.push_back(std::move(errorNode));
}

void AbstractNode::InternalAcceptVisitor(NodeVisitor& visitor, int depth) const {
    AcceptVisitor(visitor, depth);

    for (auto& error : myErrors) {
        visitor.VisitNode(*error, depth);
    }
}

void AbstractNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
}

std::ostream& operator<<(std::ostream& os, const AbstractNode& node) {
    return os << node.ToString();
}
