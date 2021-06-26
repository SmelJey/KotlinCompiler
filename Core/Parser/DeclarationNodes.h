#pragma once
#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"

class AbstractDeclaration : public AbstractNode, public virtual IAnnotatedNode {
public:
    AbstractDeclaration() = default;

    std::string GetIdentifierName() const;
    IdentifierNode& GetIdentifier() const;
    void SetIdentifier(Pointer<IdentifierNode> identifier);

    const ISymbol* GetSymbol() const override;
    void SetSymbol(const ISymbol* symbol);

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

    const ISymbol* mySymbol = nullptr;

private:
    Pointer<IdentifierNode> myIdentifier;
};

class DeclarationBlock : public AbstractNode {
public:
    DeclarationBlock(std::vector<Pointer<AbstractDeclaration>> declarations = std::vector<Pointer<AbstractDeclaration>>());

    const std::vector<Pointer<AbstractDeclaration>>& GetDeclarations() const;

    void AddDeclaration(Pointer<AbstractDeclaration> declaration);

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<AbstractDeclaration>> myDeclarations;
};

class ClassDeclaration : public AbstractDeclaration {
public:
    ClassDeclaration() = default;

    const DeclarationBlock& GetBody() const;
    void SetBody(Pointer<DeclarationBlock> body);
    bool HasBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<DeclarationBlock> myClassBody;
};

class ParameterNode : public AbstractDeclaration {
public:
    ParameterNode() = default;

    const IAnnotatedNode& GetTypeNode() const;
    void SetTypeNode(Pointer<IAnnotatedNode> typeNode);

    const IAnnotatedNode& GetDefault() const;
    void SetDefault(Pointer<IAnnotatedNode> defaultNode);
    bool HasDefault() const;

    const ITypeSymbol* GetType() const override;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myType;
    Pointer<IAnnotatedNode> myDefault;
};

class VariableNode : public AbstractDeclaration {
public:
    VariableNode() = default;

    const IAnnotatedNode& GetTypeNode() const;
    void SetTypeNode(Pointer<IAnnotatedNode> typeNode);
    bool HasTypeNode() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor & visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myType;
};

class ParameterList : public AbstractNode {
public:
    ParameterList() = default;

    const std::vector<Pointer<ParameterNode>>& GetParameters() const;
    void AddParameter(Pointer<ParameterNode> param);

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ParameterNode>> myParameters;
};

class FunctionDeclaration : public AbstractDeclaration {
public:
    FunctionDeclaration() = default;

    const ParameterList& GetParameters() const;
    void SetParameters(Pointer<ParameterList> parameters);

    const IAnnotatedNode& GetBody() const;
    void SetBody(Pointer<IAnnotatedNode> body);

    const IAnnotatedNode& GetReturn() const;
    void SetReturn(Pointer<IAnnotatedNode> returnNode);

    bool HasReturnNode() const;
protected:
    std::string GetName() const override;

    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<ParameterList> myParams;
    Pointer<IAnnotatedNode> myBody;
    Pointer<IAnnotatedNode> myReturn;
};

class PropertyDeclaration : public AbstractDeclaration {
public:
    explicit PropertyDeclaration(const Lexeme& keyword);

    bool IsMutable() const;
    std::string GetKeyword() const;

    const IAnnotatedNode& GetTypeNode() const;
    void SetTypeNode(Pointer<IAnnotatedNode> typeNode);
    bool HasTypeNode() const;

    const IAnnotatedNode& GetInitialization() const;
    void SetInitialization(Pointer<IAnnotatedNode> initNode);
    bool HasInitialization() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    Pointer<IAnnotatedNode> myType;
    Pointer<IAnnotatedNode> myInit;
    Lexeme myKeyword;
};
