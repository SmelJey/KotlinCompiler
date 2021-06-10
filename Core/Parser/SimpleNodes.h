#pragma once
#include "ISyntaxNode.h"

class ILexemeNode : public ISyntaxNode {
public:
    ILexemeNode(const Lexeme& lexeme);

    Lexeme GetLexeme() const;
protected:
    Lexeme myLexeme;
};

class IdentifierNode : public ILexemeNode {
public:
    explicit IdentifierNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class IntegerNode : public ILexemeNode {
public:
    explicit IntegerNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class RealNode : public ILexemeNode {
public:
    explicit RealNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class BooleanNode : public ILexemeNode {
public:
    explicit BooleanNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class StringNode : public ILexemeNode {
public:
    explicit StringNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class ErrorNode : public ILexemeNode {
public:
    ErrorNode(const Lexeme& lexeme, const std::string& error = "Unexpected lexeme");
protected:
    std::string GetName() const override;
private:
    std::string myError;
};

class TypeNode : public ILexemeNode {
public:
    explicit TypeNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};
