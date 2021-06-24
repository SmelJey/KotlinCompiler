#pragma once
#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"

class AbstractDeclaration : public AbstractNode, public virtual ISymbolAnnotatedNode {
public:
    AbstractDeclaration() = default;

    std::string GetIdentifierName() const;
    const IdentifierNode& GetIdentifier() const;
    void SetIdentifier(Pointer<IdentifierNode> identifier);

    const ISymbol* GetSymbol() const override;
    void SetSymbol(const ISymbol* symbol);

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<IdentifierNode> myIdentifier;
    const ISymbol* mySymbol = nullptr;
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

    const ITypedNode& GetType() const;
    void SetType(Pointer<ITypedNode> typeNode);

    const ITypedNode& GetDefault() const;
    void SetDefault(Pointer<ITypedNode> defaultNode);
    bool HasDefault() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myType;
    Pointer<ITypedNode> myDefault;
};

class VariableNode : public AbstractDeclaration {
public:
    VariableNode() = default;

    const ITypedNode& GetType() const;
    void SetType(Pointer<ITypedNode> typeNode);
    bool HasType() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor & visitor, int depth) const override;

private:
    Pointer<ITypedNode> myType;
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

    const ISyntaxNode& GetBody() const;
    void SetBody(Pointer<ISyntaxNode> body);

    const ITypedNode& GetReturn() const;
    void SetReturn(Pointer<ITypedNode> returnNode);

    bool HasReturnNode() const;
protected:
    std::string GetName() const override;

    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ParameterList> myParams;
    Pointer<ISyntaxNode> myBody;
    Pointer<ITypedNode> myReturn;
};

class PropertyDeclaration : public AbstractDeclaration {
public:
    explicit PropertyDeclaration(const Lexeme& keyword);

    bool IsMutable() const;
    std::string GetKeyword() const;

    const ITypedNode& GetType() const;
    void SetType(Pointer<ITypedNode> typeNode);
    bool HasType() const;

    const ITypedNode& GetInitialization() const;
    void SetInitialization(Pointer<ITypedNode> initNode);
    bool HasInitialization() const;

protected:
    std::string GetName() const override;
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    Pointer<ITypedNode> myType;
    Pointer<ITypedNode> myInit;
    Lexeme myKeyword;
};
