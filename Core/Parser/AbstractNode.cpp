#include "AbstractNode.h"

ISyntaxNode::~ISyntaxNode() = default;

std::string ISyntaxNode::ToString() const {
    return GetName();
}

void ISyntaxNode::RunVisitor(NodeVisitor& visitor) const {
    visitor.VisitNode(*this, 0);
}

void ISyntaxNode::InternalAcceptVisitor(NodeVisitor& visitor, int depth) const {
    AcceptVisitor(visitor, depth);

    for (auto& error : GetErrors()) {
        visitor.VisitNode(*error, depth);
    }
}

void ISyntaxNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {}

AbstractNode::AbstractNode() = default;
AbstractNode::AbstractNode(AbstractNode&& src) noexcept : myErrors(std::move(src.myErrors)) {}

AbstractNode::~AbstractNode() = default;

void AbstractNode::AddError(Pointer<ISyntaxNode> errorNode) {
    myErrors.push_back(std::move(errorNode));
}

const std::vector<Pointer<ISyntaxNode>>& AbstractNode::GetErrors() const {
    return myErrors;
}

const ITypeSymbol* ITypedNode::GetType() const {
    return dynamic_cast<const ITypeSymbol*>(GetSymbol());
}

std::ostream& operator<<(std::ostream& os, const AbstractNode& node) {
    return os << node.ToString();
}
