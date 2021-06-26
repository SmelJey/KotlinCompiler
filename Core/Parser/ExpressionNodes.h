#pragma once
#include <memory>

#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexeme.h"

class BinOperationNode : public LexemeNode, public virtual IAnnotatedNode {
public:
    BinOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> left, Pointer<IAnnotatedNode> right, const ITypeSymbol* type);

    const IAnnotatedNode& GetLeftOperand() const;
    const IAnnotatedNode& GetRightOperand() const;

    std::string GetOperation() const;

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;
protected:
    std::string GetName() const override;

    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;
private:
    Pointer<IAnnotatedNode> myLeftOperand;
    Pointer<IAnnotatedNode> myRightOperand;
    const ITypeSymbol* myType;
};

class AbstractUnaryOperationNode : public LexemeNode, public virtual IAnnotatedNode {
public:
    AbstractUnaryOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const ITypeSymbol* type);

    const IAnnotatedNode& GetOperand() const;
    std::string GetOperation() const;

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myOperand;
    const ITypeSymbol* myType;
};

class UnaryPrefixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPrefixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const ITypeSymbol* type);

protected:
    std::string GetName() const override;
};

class UnaryPostfixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPostfixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const ITypeSymbol* type);

protected:
    std::string GetName() const override;
};

class AbstractUnaryPostfixNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    AbstractUnaryPostfixNode() = default;
};

class CallArgumentsNode : public AbstractNode {
public:
    CallArgumentsNode() = default;

    const std::vector<Pointer<IAnnotatedNode>>& GetArguments() const;
    void AddArgument(Pointer<IAnnotatedNode> argument);

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<IAnnotatedNode>> myArguments;
};

class TypeArgumentsNode : public AbstractNode {
public:
    TypeArgumentsNode() = default;

    const std::vector<Pointer<TypeNode>>& GetArguments() const;
    void AddArgument(Pointer<TypeNode> argument);

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<TypeNode>> myArguments;
};

class PostfixCallNode : public AbstractUnaryPostfixNode {
public:
    PostfixCallNode(Pointer<IAnnotatedNode> expression, const ITypeSymbol* type);

    const CallArgumentsNode& GetArguments() const;
    void SetArguments(Pointer<CallArgumentsNode> arguments);

    const IAnnotatedNode* GetExpression() const;

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;
protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<CallArgumentsNode> myArgumentsNode;
    const ITypeSymbol* myType;
};

class IndexSuffixNode : public PostfixCallNode {
public:
    IndexSuffixNode(Pointer<IAnnotatedNode> expression, const ITypeSymbol* type);

    bool IsAssignable() const override;
protected:
    std::string GetName() const override;

};

class CallSuffixNode : public PostfixCallNode {
public:
    CallSuffixNode(Pointer<IAnnotatedNode> expression, const ITypeSymbol* type);

    const TypeArgumentsNode& GetTypeArguments() const;
    void SetTypeArguments(Pointer<TypeArgumentsNode> arguments);
    bool HasTypeArguments() const;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

    std::string GetName() const override;

private:
    Pointer<TypeArgumentsNode> myTypeArguments;
};

class MemberAccessNode : public AbstractUnaryPostfixNode {
public:
    MemberAccessNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<IAnnotatedNode> member);

    std::string GetOperation() const;

    const IAnnotatedNode* GetExpression() const;
    const IAnnotatedNode& GetMember() const;

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;
    bool IsAssignable() const override;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Lexeme myOperation;
    Pointer<IAnnotatedNode> myExpression;
    Pointer<IAnnotatedNode> myMemberNode;
};

class IfExpression : public AbstractNode, public virtual IAnnotatedNode {
public:
    IfExpression(const ITypeSymbol* type);

    const IAnnotatedNode* GetExpression() const;
    void SetExpression(Pointer<IAnnotatedNode> expression);

    const ISyntaxNode* GetIfBody() const;
    void SetIfBody(Pointer<ISyntaxNode> body);
    bool HasIfBody() const;

    const ISyntaxNode* GetElseBody() const;
    void SetElseBody(Pointer<ISyntaxNode> body);
    bool HasElseBody() const;

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<ISyntaxNode> myIfBody;
    Pointer<ISyntaxNode> myElseBody;

    const ITypeSymbol* myType;
};

class BlockNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    BlockNode() = default;

    const std::vector<Pointer<ISyntaxNode>>& GetStatements() const;
    void AddStatement(Pointer<ISyntaxNode> statement);

    const ISymbol* GetSymbol() const override;
    const ITypeSymbol* GetType() const override;
    void SetSymbol(const ITypeSymbol* returnSym);

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ISyntaxNode>> myStatements;
    const ITypeSymbol* myReturn;
};
