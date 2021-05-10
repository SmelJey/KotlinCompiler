#pragma once
#include "ISyntaxNode.h"

class SimpleNode : public ISyntaxNode {
public:
    SimpleNode(const Lexeme& lexeme);

   // virtual ~SimpleNode() override;

    Lexeme GetLexeme() const;
protected:
    Lexeme myLexeme;
};

class IdentifierNode : public SimpleNode {
public:
    IdentifierNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class IntegerNode : public SimpleNode {
public:
    IntegerNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};
