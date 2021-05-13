#include "SimpleNodes.h"

#include <utility>

SimpleNode::SimpleNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

//SimpleNode::~SimpleNode() = default;

Lexeme SimpleNode::GetLexeme() const {
    return myLexeme;
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme) : SimpleNode(lexeme) {}

std::string IdentifierNode::GetName() const {
    return "Identifier " + myLexeme.GetValue<std::string>();
}

IntegerNode::IntegerNode(const Lexeme& lexeme) : SimpleNode(lexeme) {}

std::string IntegerNode::GetName() const {
    return "Integer " + std::to_string(myLexeme.GetValue<uint64_t>());
}
