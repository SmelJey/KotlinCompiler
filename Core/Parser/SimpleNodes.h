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

class UnexpectedNode : public SimpleNode {
public:
    UnexpectedNode(const Lexeme& lexeme, const std::string& error = "Unexpected lexeme");
protected:
    std::string GetName() const override;
private:
    std::string myError;
};

class ErrorNode : public ISyntaxNode {
public:
    ErrorNode(const std::string& errorMessage, size_t row, size_t col);
protected:
    std::string GetName() const override;
private:
    std::string myError;
    size_t myRow;
    size_t myCol;
};
