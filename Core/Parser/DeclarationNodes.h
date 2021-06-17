#pragma once
#include "AbstractNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"

class AbstractDeclaration : public AbstractNode {
public:
    AbstractDeclaration() = default;

    std::string GetIdentifierName() const;
    const IdentifierNode& GetIdentifier() const;
    void SetIdentifier(Pointer<IdentifierNode> identifier);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

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
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

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
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<DeclarationBlock> myClassBody;
};

class ParameterNode : public AbstractDeclaration {
public:
    ParameterNode() = default;

    const AbstractNode& GetType() const;
    void SetType(Pointer<AbstractNode> typeNode);

    const AbstractNode& GetDefault() const;
    void SetDefault(Pointer<AbstractNode> defaultNode);
    bool HasDefault() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myType;
    Pointer<AbstractNode> myDefault;
};

class VariableNode : public AbstractDeclaration {
public:
    VariableNode() = default;

    const AbstractNode& GetType() const;
    void SetType(Pointer<AbstractNode> typeNode);
    bool HasType() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor & visitor, int depth) const override;

private:
    Pointer<AbstractNode> myType;
};

class ParameterList : public AbstractNode {
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

class FunctionDeclaration : public AbstractDeclaration {
public:
    FunctionDeclaration() = default;

    const ParameterList& GetParameters() const;
    void SetParameters(Pointer<ParameterList> parameters);

    const AbstractNode& GetBody() const;
    void SetBody(Pointer<AbstractNode> body);

    const AbstractNode& GetReturn() const;
    void SetReturn(Pointer<AbstractNode> returnNode);

    bool HasReturnNode() const;
protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ParameterList> myParams;
    Pointer<AbstractNode> myBody;
    Pointer<AbstractNode> myReturn;
};

class PropertyDeclaration : public AbstractDeclaration {
public:
    explicit PropertyDeclaration(const Lexeme& keyword);

    bool IsMutable() const;
    std::string GetKeyword() const;

    const AbstractNode& GetType() const;
    void SetType(Pointer<AbstractNode> typeNode);
    bool HasType() const;

    const AbstractNode& GetInitialization() const;
    void SetInitialization(Pointer<AbstractNode> initNode);
    bool HasInitialization() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<AbstractNode> myType;
    Pointer<AbstractNode> myInit;
    Lexeme myKeyword;
};
