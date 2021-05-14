#include "SimpleNodes.h"

#include <sstream>

SimpleNode::SimpleNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

Lexeme SimpleNode::GetLexeme() const {
    return myLexeme;
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme) : SimpleNode(lexeme) {}

std::string IdentifierNode::GetName() const {
    return "Identifier :: " + myLexeme.GetValue<std::string>();
}

IntegerNode::IntegerNode(const Lexeme& lexeme) : SimpleNode(lexeme) {}

std::string IntegerNode::GetName() const {
    return "Integer :: " + std::to_string(myLexeme.GetValue<uint64_t>());
}

ErrorNode::ErrorNode(const Lexeme& lexeme, const std::string& error) : SimpleNode(lexeme), myError(error) {}

std::string ErrorNode::GetName() const {
    std::stringstream ss;
    ss << "Error " << myLexeme.GetRow() << ":" << myLexeme.GetColumn() << " :: " + myError;
    return ss.str();
}
