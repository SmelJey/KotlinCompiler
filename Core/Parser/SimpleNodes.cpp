#include "SimpleNodes.h"

#include <sstream>

ILexemeNode::ILexemeNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

Lexeme ILexemeNode::GetLexeme() const {
    return myLexeme;
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string IdentifierNode::GetName() const {
    return "Identifier :: " + myLexeme.GetValue<std::string>();
}

IntegerNode::IntegerNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string IntegerNode::GetName() const {
    return "Integer :: " + std::to_string(myLexeme.GetValue<uint64_t>());
}

ErrorNode::ErrorNode(const Lexeme& lexeme, const std::string& error) : ILexemeNode(lexeme), myError(error) {}

std::string ErrorNode::GetName() const {
    std::stringstream ss;
    ss << "Error " << myLexeme.GetRow() << ":" << myLexeme.GetColumn() << " :: " + myError;
    return ss.str();
}

TypeNode::TypeNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string TypeNode::GetName() const {
    return "Type :: " + myLexeme.GetValue<std::string>();
}
