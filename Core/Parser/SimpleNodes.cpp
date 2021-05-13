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

UnexpectedNode::UnexpectedNode(const Lexeme& lexeme, const std::string& error) : SimpleNode(lexeme), myError(error) {}

std::string UnexpectedNode::GetName() const {
    return "Error :: " + myError;
}

ErrorNode::ErrorNode(const std::string& errorMessage, size_t row, size_t col) : myError(errorMessage), myRow(row), myCol(col) {}

std::string ErrorNode::GetName() const {
    std::stringstream ss;
    ss << "Error " << myRow << ":" << myCol << " :: " + myError;
    return ss.str();
}
