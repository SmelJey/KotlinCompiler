#pragma once
#include "ISyntaxNode.h"
#include "SimpleNodes.h"

class IDeclaration : public ISyntaxNode {
public:
    IDeclaration() = default;

    std::string GetIdentifier() const;
    const IdentifierNode& GetIdentifierNode() const;
    void SetIdentifier(std::unique_ptr<IdentifierNode> identifier);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<IdentifierNode> myIdentifier;
};

class DeclarationBlock : public ISyntaxNode {
public:
    DeclarationBlock(std::vector<std::unique_ptr<IDeclaration>> declarations = std::vector<std::unique_ptr<IDeclaration>>());

    const std::vector<std::unique_ptr<IDeclaration>>& GetDeclarations() const;

    void AddDeclaration(std::unique_ptr<IDeclaration> declaration);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<std::unique_ptr<IDeclaration>> myDeclarations;
};

class ClassDeclaration : public IDeclaration {
public:
    ClassDeclaration() = default;

    const DeclarationBlock& GetBody() const;
    void SetBody(std::unique_ptr<DeclarationBlock> body);
    bool HasBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<DeclarationBlock> myClassBody;
};

class ParameterNode : public ISyntaxNode {
public:
    ParameterNode() = default;

    const IdentifierNode& GetIdentifier() const;
    void SetIdentifier(std::unique_ptr<IdentifierNode> identifier);

    const ISyntaxNode& GetTypeNode() const;
    void SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode);

    const ISyntaxNode& GetDefaultNode() const;
    void SetDefaultNode(std::unique_ptr<ISyntaxNode> defaultNode);
    bool HasDefaultNode() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<IdentifierNode> myIdentifierNode;
    std::unique_ptr<ISyntaxNode> myTypeNode;
    std::unique_ptr<ISyntaxNode> myDefaultNode;
};

class VariableNode : public ISyntaxNode {
public:
    VariableNode() = default;

    const IdentifierNode& GetIdentifier() const;
    void SetIdentifier(std::unique_ptr<IdentifierNode> identifier);

    const ISyntaxNode& GetTypeNode() const;
    void SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode);
    bool HasTypeNode() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor & visitor, int depth) const override;

private:
    std::unique_ptr<IdentifierNode> myIdentifierNode;
    std::unique_ptr<ISyntaxNode> myTypeNode;
};

class ParameterList : public ISyntaxNode {
public:
    ParameterList() = default;

    const std::vector<std::unique_ptr<ParameterNode>>& GetParameters() const;
    void AddParameter(std::unique_ptr<ParameterNode> param);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<std::unique_ptr<ParameterNode>> myParameters;
};

class FunctionDeclaration : public IDeclaration {
public:
    FunctionDeclaration() = default;

    const ParameterList& GetParameters() const;
    void SetParameters(std::unique_ptr<ParameterList> parameters);

    const ISyntaxNode& GetBody() const;
    void SetBody(std::unique_ptr<ISyntaxNode> body);

    const ISyntaxNode& GetReturnNode() const;
    void SetReturnNode(std::unique_ptr<ISyntaxNode> returnNode);

    bool HasReturnNode() const;
protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ParameterList> myParams;
    std::unique_ptr<ISyntaxNode> myBody;
    std::unique_ptr<ISyntaxNode> myReturnNode;
};

class PropertyDeclaration : public IDeclaration {
public:
    explicit PropertyDeclaration(const Lexeme& keyword);

    bool IsMutable() const;
    std::string GetKeyword() const;

    const ISyntaxNode& GetType() const;
    void SetType(std::unique_ptr<ISyntaxNode> typeNode);
    bool HasType() const;

    const ISyntaxNode& GetInitialization() const;
    void SetInitialization(std::unique_ptr<ISyntaxNode> initNode);
    bool HasInitialization() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myTypeNode;
    std::unique_ptr<ISyntaxNode> myInit;
    Lexeme myKeyword;
};
