#pragma once
#include "NodeVisitor.h"

class IVisitable {
public:
    virtual ~IVisitable() = default;

    friend class NodeVisitor;
protected:
    void InternalAcceptVisitor(NodeVisitor& visitor, int depth) const;
    virtual void AcceptVisitor(NodeVisitor& visitor, int depth) const;
};
