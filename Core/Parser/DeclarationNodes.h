#pragma once
#include "ISyntaxNode.h"
#include "SimpleNodes.h"

class IDeclaration : public ILexemeNode {
public:
    explicit IDeclaration(const Lexeme& identifierLexeme);

    std::string GetIdentifier() const;
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
    ClassDeclaration(const Lexeme& lexeme, std::unique_ptr<DeclarationBlock> body);

    const DeclarationBlock& GetClassBody() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<DeclarationBlock> myClassBody;
};

class Parameter : public ILexemeNode {
public:
    explicit Parameter(const Lexeme& parameterLexeme);

    const ISyntaxNode& GetTypeNode() const;
    void SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode);

    const ISyntaxNode& GetDefaultNode() const;
    void SetDefaultNode(std::unique_ptr<ISyntaxNode> defaultNode);

    bool HasDefaultNode() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::unique_ptr<ISyntaxNode> myTypeNode;
    std::unique_ptr<ISyntaxNode> myDefaultNode;
};

class ParameterList : public ISyntaxNode {
public:
    ParameterList() = default;

    const std::vector<std::unique_ptr<Parameter>>& GetParameters() const;
    void AddParameter(std::unique_ptr<Parameter> param);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::vector<std::unique_ptr<Parameter>> myParameters;
};

class FunctionDeclaration : public IDeclaration {
public:
    explicit FunctionDeclaration(const Lexeme& lexeme);

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

class BlockNode : public ISyntaxNode {
public:
    BlockNode() = default;

    const std::vector<std::unique_ptr<ISyntaxNode>>& GetStatements() const;
    void AddStatement(std::unique_ptr<ISyntaxNode> statement);

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    // TODO: create statement node
    std::vector<std::unique_ptr<ISyntaxNode>> myStatements;
};