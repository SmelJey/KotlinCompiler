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

RealNode::RealNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string RealNode::GetName() const {
    return "Real :: " + std::to_string(myLexeme.GetValue<double>());
}

BooleanNode::BooleanNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string BooleanNode::GetName() const {
    return "Boolean :: " + myLexeme.GetValue<std::string>();
}

StringNode::StringNode(const Lexeme& lexeme) : ILexemeNode(lexeme) {}

std::string StringNode::GetName() const {
    return "String :: " + myLexeme.GetValue<std::string>();
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
