#include "SimpleNodes.h"

SimpleNode::SimpleNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

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

UnexpectedNode::UnexpectedNode(const Lexeme& lexeme, const std::string& error) : SimpleNode(lexeme), myError(error) {}

std::string UnexpectedNode::GetName() const {
    return "Error :: " + myError;
}

ErrorNode::ErrorNode(const std::string& errorMessage) : myError(errorMessage) {}

std::string ErrorNode::GetName() const {
    return "Error :: " + myError;
}
