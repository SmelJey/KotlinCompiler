#include "IVisitable.h"

void IVisitable::InternalAcceptVisitor(NodeVisitor& visitor, int depth) const {
    AcceptVisitor(visitor, depth);
}

void IVisitable::AcceptVisitor(NodeVisitor& visitor, int depth) const {}
