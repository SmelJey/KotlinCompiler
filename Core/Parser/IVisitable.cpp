#include "IVisitable.h"
#include "NodeVisitor.h"

IVisitable::~IVisitable() = default;

void IVisitable::RunVisitor(NodeVisitor & visitor) const {
    visitor.VisitNode(*this, 0);
}

void IVisitable::AcceptVisitor(NodeVisitor & visitor, int depth) const {}

