#include "IVisitable.h"
#include "INodeVisitor.h"

IVisitable::~IVisitable() = default;

void IVisitable::RunVisitor(INodeVisitor & visitor) const {
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

void IVisitable::AcceptVisitor(INodeVisitor & visitor) const {
}

