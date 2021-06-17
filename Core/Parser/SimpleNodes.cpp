#include "SimpleNodes.h"

#include <sstream>

LexemeNode::LexemeNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

Lexeme LexemeNode::GetLexeme() const {
    return myLexeme;
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string IdentifierNode::GetName() const {
    return "Identifier :: " + myLexeme.GetValue<std::string>();
}

IntegerNode::IntegerNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string IntegerNode::GetName() const {
    return "Integer :: " + std::to_string(myLexeme.GetValue<uint64_t>());
}

RealNode::RealNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string RealNode::GetName() const {
    return "Real :: " + std::to_string(myLexeme.GetValue<double>());
}

BooleanNode::BooleanNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string BooleanNode::GetName() const {
    return "Boolean :: " + myLexeme.GetValue<std::string>();
}

StringNode::StringNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string StringNode::GetName() const {
    return "String :: " + myLexeme.GetValue<std::string>();
}

ErrorNode::ErrorNode(const Lexeme& lexeme, const std::string& error) : LexemeNode(lexeme), myError(error) {}

std::string ErrorNode::GetName() const {
    std::stringstream ss;
    ss << "Error " << myLexeme.GetRow() << ":" << myLexeme.GetColumn() << " :: " + myError;
    return ss.str();
}

TypeNode::TypeNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string TypeNode::GetName() const {
    return "Type :: " + myLexeme.GetValue<std::string>();
}

BreakNode::BreakNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string BreakNode::GetName() const {
    return "Break";
}

ContinueNode::ContinueNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string ContinueNode::GetName() const {
    return "Continue";
}

ReturnNode::ReturnNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

const AbstractNode* ReturnNode::GetExpression() const {
    return myExpression.get();
}

void ReturnNode::SetExpression(std::unique_ptr<AbstractNode> expression) {
    myExpression = std::move(expression);
}

bool ReturnNode::HasExpression() const {
    return myExpression != nullptr;
}

std::string ReturnNode::GetName() const {
    return "Return";
}

void ReturnNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    if (HasExpression()) {
        visitor.VisitNode(*myExpression, depth);
    }
}
