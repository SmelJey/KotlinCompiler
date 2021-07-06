#pragma once
#include <memory>

#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexeme.h"

class BinOperationNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    BinOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> left, Pointer<IAnnotatedNode> right, const AbstractType* type);

    const IAnnotatedNode& GetLeftOperand() const;
    const IAnnotatedNode& GetRightOperand() const;

    std::string GetOperation() const;

    const ISymbol* GetSymbol() const override;
    const AbstractType* GetType() const override;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;

    void PropagateVisitor(INodeVisitor& visitor) const override;
private:
    Pointer<IAnnotatedNode> myLeftOperand;
    Pointer<IAnnotatedNode> myRightOperand;
    const AbstractType* myType;
};

class AbstractUnaryOperationNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    AbstractUnaryOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const AbstractType* type);

    const IAnnotatedNode& GetOperand() const;
    std::string GetOperation() const;

    const ISymbol* GetSymbol() const override;
    const AbstractType* GetType() const override;

protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myOperand;
    const AbstractType* myType;
};

class UnaryPrefixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPrefixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const AbstractType* type);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
};

class UnaryPostfixOperationNode : public AbstractUnaryOperationNode {
public:
    UnaryPostfixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const AbstractType* type);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
};

class AbstractUnaryPostfixNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    explicit AbstractUnaryPostfixNode(const Lexeme& lexeme);
};

class CallArgumentsNode : public AbstractNode {
public:
    explicit CallArgumentsNode(const Lexeme& lexeme);

    const std::vector<Pointer<IAnnotatedNode>>& GetArguments() const;
    void AddArgument(Pointer<IAnnotatedNode> argument);

    const std::vector<const AbstractType*> GetTypes() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    std::vector<Pointer<IAnnotatedNode>> myArguments;
};

class TypeArgumentsNode : public AbstractNode {
public:
    explicit TypeArgumentsNode(const Lexeme& lexeme);

    const std::vector<Pointer<TypeNode>>& GetArguments() const;
    void AddArgument(Pointer<TypeNode> argument);

    const std::vector<const AbstractType*> GetTypes() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    std::vector<Pointer<TypeNode>> myArguments;
};

class AbstractPostfixCallNode : public AbstractUnaryPostfixNode {
public:
    AbstractPostfixCallNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const AbstractType* type);

    const CallArgumentsNode& GetArguments() const;

    const IAnnotatedNode* GetExpression() const;

    const ISymbol* GetSymbol() const override;
    const AbstractType* GetType() const override;
protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<CallArgumentsNode> myArgumentsNode;
    const AbstractType* myType;
};

class IndexSuffixNode : public AbstractPostfixCallNode {
public:
    IndexSuffixNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const AbstractType* type);

    bool IsAssignable() const override;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;

};

class CallSuffixNode : public AbstractPostfixCallNode {
public:
    CallSuffixNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const AbstractType* type);

    const TypeArgumentsNode& GetTypeArguments() const;
    void SetTypeArguments(Pointer<TypeArgumentsNode> arguments);
    bool HasTypeArguments() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

    std::string GetName() const override;

private:
    Pointer<TypeArgumentsNode> myTypeArguments;
};

class MemberAccessNode : public AbstractUnaryPostfixNode {
public:
    MemberAccessNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<IAnnotatedNode> member);

    std::string GetOperation() const;

    const IAnnotatedNode* GetExpression() const;
    IAnnotatedNode* GetMember() const;

    const ISymbol* GetSymbol() const override;
    const AbstractType* GetType() const override;
    bool IsAssignable() const override;

    void RunVisitor(INodeVisitor& visitor) const override;

protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<IAnnotatedNode> myMemberNode;
};

class IfExpression : public UnitTypedNode {
public:
    IfExpression(const Lexeme& lexeme, const UnitTypeSymbol* type, Pointer<IAnnotatedNode> expression, Pointer<IAnnotatedNode> ifBody, Pointer<IAnnotatedNode> elseBody);

    const IAnnotatedNode* GetExpression() const;

    const IAnnotatedNode* GetIfBody() const;
    const IAnnotatedNode* GetElseBody() const;

    const ISymbol* GetSymbol() const override;
    const AbstractType* GetType() const override;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myExpression;
    Pointer<IAnnotatedNode> myIfBody;
    Pointer<IAnnotatedNode> myElseBody;
};

class BlockNode : public AbstractNode, public virtual IAnnotatedNode {
public:
    BlockNode(const Lexeme& lexeme, const UnitTypeSymbol* type);

    const std::vector<Pointer<IAnnotatedNode>>& GetStatements() const;
    void AddStatement(Pointer<IAnnotatedNode> statement);

    const ISymbol* GetSymbol() const override;
    const AbstractType* GetType() const override;
    void SetSymbol(const AbstractType* returnSym);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    std::vector<Pointer<IAnnotatedNode>> myStatements;
    const AbstractType* myReturn;
};
