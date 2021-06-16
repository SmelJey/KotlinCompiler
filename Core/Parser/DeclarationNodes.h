#pragma once
#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"

class IDeclaration : public ISyntaxNode {
public:
    IDeclaration() = default;

    std::string GetIdentifier() const;
    const IdentifierNode& GetIdentifierNode() const;
    void SetIdentifier(Pointer<IdentifierNode> identifier);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<IdentifierNode> myIdentifier;
};

class DeclarationBlock : public ISyntaxNode {
public:
    DeclarationBlock(std::vector<Pointer<IDeclaration>> declarations = std::vector<Pointer<IDeclaration>>());

    const std::vector<Pointer<IDeclaration>>& GetDeclarations() const;

    void AddDeclaration(Pointer<IDeclaration> declaration);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<IDeclaration>> myDeclarations;
};

class ClassDeclaration : public IDeclaration {
public:
    ClassDeclaration() = default;

    const DeclarationBlock& GetBody() const;
    void SetBody(Pointer<DeclarationBlock> body);
    bool HasBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<DeclarationBlock> myClassBody;
};

class ParameterNode : public ISyntaxNode {
public:
    ParameterNode() = default;

    const IdentifierNode& GetIdentifier() const;
    void SetIdentifier(Pointer<IdentifierNode> identifier);

    const ISyntaxNode& GetTypeNode() const;
    void SetTypeNode(Pointer<ISyntaxNode> typeNode);

    const ISyntaxNode& GetDefaultNode() const;
    void SetDefaultNode(Pointer<ISyntaxNode> defaultNode);
    bool HasDefaultNode() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<IdentifierNode> myIdentifier;
    Pointer<ISyntaxNode> myType;
    Pointer<ISyntaxNode> myDefault;
};

class VariableNode : public ISyntaxNode {
public:
    VariableNode() = default;

    const IdentifierNode& GetIdentifier() const;
    void SetIdentifier(Pointer<IdentifierNode> identifier);

    const ISyntaxNode& GetTypeNode() const;
    void SetTypeNode(Pointer<ISyntaxNode> typeNode);
    bool HasTypeNode() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor & visitor, int depth) const override;

private:
    Pointer<IdentifierNode> myIdentifier;
    Pointer<ISyntaxNode> myType;
};

class ParameterList : public ISyntaxNode {
public:
    ParameterList() = default;

    const std::vector<Pointer<ParameterNode>>& GetParameters() const;
    void AddParameter(Pointer<ParameterNode> param);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<Pointer<ParameterNode>> myParameters;
};

class FunctionDeclaration : public IDeclaration {
public:
    FunctionDeclaration() = default;

    const ParameterList& GetParameters() const;
    void SetParameters(Pointer<ParameterList> parameters);

    const ISyntaxNode& GetBody() const;
    void SetBody(Pointer<ISyntaxNode> body);

    const ISyntaxNode& GetReturnNode() const;
    void SetReturn(Pointer<ISyntaxNode> returnNode);

    bool HasReturnNode() const;
protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ParameterList> myParams;
    Pointer<ISyntaxNode> myBody;
    Pointer<ISyntaxNode> myReturn;
};

class PropertyDeclaration : public IDeclaration {
public:
    explicit PropertyDeclaration(const Lexeme& keyword);

    bool IsMutable() const;
    std::string GetKeyword() const;

    const ISyntaxNode& GetType() const;
    void SetType(Pointer<ISyntaxNode> typeNode);
    bool HasType() const;

    const ISyntaxNode& GetInitialization() const;
    void SetInitialization(Pointer<ISyntaxNode> initNode);
    bool HasInitialization() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ISyntaxNode> myType;
    Pointer<ISyntaxNode> myInit;
    Lexeme myKeyword;
};
