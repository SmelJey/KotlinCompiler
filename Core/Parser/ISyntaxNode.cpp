#include "ISyntaxNode.h"

std::string ISyntaxNode::ToString() const {
    return GetName();
}

AbstractNode::AbstractNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

AbstractNode::AbstractNode(AbstractNode&& src) noexcept {}

Lexeme AbstractNode::GetLexeme() const {
    return myLexeme;
}

const AbstractType* IAnnotatedNode::GetType() const {
    return nullptr;
}

bool IAnnotatedNode::IsAssignable() const {
    return false;
}

std::ostream& operator<<(std::ostream& os, const AbstractNode& node) {
    return os << node.ToString();
}
