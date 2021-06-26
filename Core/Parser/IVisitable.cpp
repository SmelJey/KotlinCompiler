#include "IVisitable.h"
#include "INodeVisitor.h"

IVisitable::~IVisitable() = default;

void IVisitable::RunVisitor(INodeVisitor & visitor) const {
    visitor.VisitNode(*this, 0);
}

void IVisitable::AcceptVisitor(INodeVisitor & visitor, int depth) const {}

