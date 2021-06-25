#pragma once
#include <memory>

#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexeme.h"

class BinOperationNode : public LexemeNode, public virtual ITypedNode {
public:
    BinOperationNode(const Lexeme& operation, Pointer<ITypedNode> left, Pointer<ITypedNode> right, const ITypeSymbol* type);

    const ITypedNode& GetLeftOperand() const;
    const ITypedNode& GetRightOperand() const;

    std::string GetOperation() const;

    const ISymbol* GetSymbol() const override;

protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;
private:
    Pointer<ITypedNode> myLeftOperand;
    Pointer<ITypedNode> myRightOperand;
    const ITypeSymbol* myType;
};

class AbstractUnaryOperationNode : public LexemeNode, public virtual ITypedNode {
public:
    AbstractUnaryOperationNode(const Lexeme& operation, Pointer<ITypedNode> operand, const ITypeSymbol* type);

    const ITypedNode& GetOperand() const;
    std::string GetOperation() const;

    const ISymbol* GetSymbol() const override;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myOperand;
    const ITypeSymbol* myType;
};

class UnaryPrefixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPrefixOperationNode(const Lexeme& operation, Pointer<ITypedNode> operand, const ITypeSymbol* type);

protected:
    std::string GetName() const override;
};

class UnaryPostfixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPostfixOperationNode(const Lexeme& operation, Pointer<ITypedNode> operand, const ITypeSymbol* type);

protected:
    std::string GetName() const override;
};

class AbstractUnaryPostfixNode : public AbstractNode, public virtual ITypedNode {
public:
    AbstractUnaryPostfixNode() = default;
};

class CallArgumentsNode : public AbstractNode {
public:
    CallArgumentsNode() = default;

    const std::vector<Pointer<ITypedNode>>& GetArguments() const;
    void AddArgument(Pointer<ITypedNode> argument);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ITypedNode>> myArguments;
};

class TypeArgumentsNode : public AbstractNode {
public:
    TypeArgumentsNode() = default;

    const std::vector<Pointer<TypeNode>>& GetArguments() const;
    void AddArgument(Pointer<TypeNode> argument);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<TypeNode>> myArguments;
};

class PostfixCallNode : public AbstractUnaryPostfixNode {
public:
    PostfixCallNode(Pointer<ITypedNode> expression, const ITypeSymbol* type);

    const CallArgumentsNode& GetArguments() const;
    void SetArguments(Pointer<CallArgumentsNode> arguments);

    const ITypedNode* GetExpression() const;

    const ISymbol* GetSymbol() const override;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myExpression;
    Pointer<CallArgumentsNode> myArgumentsNode;
    const ITypeSymbol* myType;
};

class IndexSuffixNode : public PostfixCallNode {
public:
    IndexSuffixNode(Pointer<ITypedNode> expression, const ITypeSymbol* type);

protected:
    std::string GetName() const override;

};

class CallSuffixNode : public PostfixCallNode {
public:
    CallSuffixNode(Pointer<ITypedNode> expression, const ITypeSymbol* type);

    const TypeArgumentsNode& GetTypeArguments() const;
    void SetTypeArguments(Pointer<TypeArgumentsNode> arguments);
    bool HasTypeArguments() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

    std::string GetName() const override;

private:
    Pointer<TypeArgumentsNode> myTypeArguments;
};

class MemberAccessNode : public AbstractUnaryPostfixNode {
public:
    MemberAccessNode(const Lexeme& lexeme, Pointer<ITypedNode> expression, Pointer<ITypedNode> member);

    std::string GetOperation() const;

    const ITypedNode* GetExpression() const;
    const ITypedNode& GetMember() const;

    const ISymbol* GetSymbol() const override;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Lexeme myOperation;
    Pointer<ITypedNode> myExpression;
    Pointer<ITypedNode> myMemberNode;
};

class IfExpression : public AbstractNode, public virtual ITypedNode {
public:
    IfExpression(const ITypeSymbol* type);

    const ITypedNode* GetExpression() const;
    void SetExpression(Pointer<ITypedNode> expression);

    const ISyntaxNode* GetIfBody() const;
    void SetIfBody(Pointer<ISyntaxNode> body);
    bool HasIfBody() const;

    const ISyntaxNode* GetElseBody() const;
    void SetElseBody(Pointer<ISyntaxNode> body);
    bool HasElseBody() const;

    const ISymbol* GetSymbol() const override;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myExpression;
    Pointer<ISyntaxNode> myIfBody;
    Pointer<ISyntaxNode> myElseBody;

    const ITypeSymbol* myType;
};

class BlockNode : public AbstractNode, public virtual ITypedNode {
public:
    BlockNode() = default;

    const std::vector<Pointer<ISyntaxNode>>& GetStatements() const;
    void AddStatement(Pointer<ISyntaxNode> statement);

    const ISymbol* GetSymbol() const override;
    void SetSymbol(const ITypeSymbol* returnSym);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ISyntaxNode>> myStatements;
    const ITypeSymbol* myReturn;
};
