#pragma once
#include <string>

class INodeVisitor;

class IVisitable {
public:
    virtual ~IVisitable();

    friend class INodeVisitor;

    virtual std::string ToString() const = 0;
    void RunVisitor(INodeVisitor& visitor) const;

protected:
    virtual void AcceptVisitor(INodeVisitor& visitor, int depth) const;
};