#pragma once

#include "../INodeVisitor.h"
#include "../ParserError.h"

class SemanticsChecker : public INodeVisitor {
public:
    SemanticsChecker(std::vector<ParserError>& errors, bool wasError);

protected:
    void EnterNode(const IVisitable& node, int depth) override;
    void ExitNode(const IVisitable& node, int depth) override;

private:
    std::vector<ParserError>& myErrors;
    bool isError = false;
};