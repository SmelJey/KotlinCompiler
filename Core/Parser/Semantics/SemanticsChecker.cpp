#include "SemanticsChecker.h"
#include "../ISyntaxNode.h"

SemanticsChecker::SemanticsChecker(std::vector<ParserError>& errors) : myErrors(errors) {}

void SemanticsChecker::EnterNode(const IVisitable& node, int depth) {
    auto typedNode = dynamic_cast<const IAnnotatedNode*>(&node);
    if (typedNode != nullptr && *typedNode->GetSymbol() == UnresolvedSymbol()) {
        //myErrors.emplace_back();
    }
}

void SemanticsChecker::ExitNode(const IVisitable& node, int depth) {}
