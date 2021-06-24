#pragma once
#include <string>

class NodeVisitor;

class IVisitable {
public:
    virtual ~IVisitable();

    friend class NodeVisitor;

    virtual std::string ToString() const = 0;
    void RunVisitor(NodeVisitor& visitor) const;

protected:
    virtual void AcceptVisitor(NodeVisitor& visitor, int depth) const;
};