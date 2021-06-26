#pragma once

#include "../INodeVisitor.h"
#include "../ParserError.h"

class SemanticsChecker : public INodeVisitor {
public:
    explicit SemanticsChecker(std::vector<ParserError>& errors);

protected:
    void EnterNode(const IVisitable& node, int depth) override;
    void ExitNode(const IVisitable& node, int depth) override;

private:
    std::vector<ParserError>& myErrors;

};