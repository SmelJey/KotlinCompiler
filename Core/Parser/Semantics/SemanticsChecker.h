#pragma once

#include "../INodeVisitor.h"

class SemanticsChecker : public INodeVisitor {
public:

protected:
    void EnterNode(const IVisitable& node, int depth) override;
    void ExitNode(const IVisitable& node, int depth) override;

private:


};