#pragma once
#include "AbstractNode.h"
#include "ParserUtils.h"

class LexemeNode : public AbstractNode {
public:
    LexemeNode(const Lexeme& lexeme);

    Lexeme GetLexeme() const;
protected:
    Lexeme myLexeme;
};

class IdentifierNode : public LexemeNode {
public:
    explicit IdentifierNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class IntegerNode : public LexemeNode {
public:
    explicit IntegerNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class RealNode : public LexemeNode {
public:
    explicit RealNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class BooleanNode : public LexemeNode {
public:
    explicit BooleanNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class StringNode : public LexemeNode {
public:
    explicit StringNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class ErrorNode : public LexemeNode {
public:
    ErrorNode(const Lexeme& lexeme, const std::string& error = "Unexpected lexeme");
protected:
    std::string GetName() const override;
private:
    std::string myError;
};

class TypeNode : public LexemeNode {
public:
    explicit TypeNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class BreakNode : public LexemeNode {
public:
    explicit BreakNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class ContinueNode : public LexemeNode {
public:
    explicit ContinueNode(const Lexeme& lexeme);
protected:
    std::string GetName() const override;
};

class ReturnNode : public LexemeNode {
public:
    explicit ReturnNode(const Lexeme& lexeme);

    const AbstractNode* GetExpression() const;
    void SetExpression(Pointer<AbstractNode> expression);
    bool HasExpression() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myExpression;
};
