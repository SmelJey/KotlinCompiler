#include "SemanticsChecker.h"
#include "../ISyntaxNode.h"

SemanticsChecker::SemanticsChecker(std::vector<ParserError>& errors, bool wasError) : myErrors(errors), isError(wasError) {}

void SemanticsChecker::EnterNode(const IVisitable& node, int depth) {}

void SemanticsChecker::ExitNode(const IVisitable& node, int depth) {
    if (isError) {
        return;
    }

    auto typedNode = dynamic_cast<const IAnnotatedNode*>(&node);
    if (typedNode != nullptr && typedNode->GetSymbol() != nullptr && *typedNode->GetSymbol() == UnresolvedSymbol()) {
        myErrors.emplace_back(typedNode->GetLexeme(), "Unresolved symbol");
        isError = true;
    }

}
