#pragma once
#include "ISyntaxNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"

class AbstractDeclaration : public AbstractNode, public virtual IAnnotatedNode {
public:
    explicit AbstractDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type);

    std::string GetIdentifierName() const;
    IdentifierNode& GetIdentifier() const;

    const ISymbol* GetSymbol() const override;
    void SetSymbol(const ISymbol* symbol);
    const AbstractType* GetType() const override;

    Lexeme GetLexeme() const override;

protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

    const ISymbol* mySymbol = nullptr;
    const UnitTypeSymbol* myUnitSym;

private:
    Pointer<IdentifierNode> myIdentifier;
};

class DeclarationBlock : public AbstractNode {
public:
    DeclarationBlock(const Lexeme& lexeme, std::vector<Pointer<AbstractDeclaration>> declarations = std::vector<Pointer<AbstractDeclaration>>());

    const std::vector<Pointer<AbstractDeclaration>>& GetDeclarations() const;

    void AddDeclaration(Pointer<AbstractDeclaration> declaration);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    std::vector<Pointer<AbstractDeclaration>> myDeclarations;
};

class ClassDeclaration : public AbstractDeclaration {
public:
    explicit ClassDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type);

    const DeclarationBlock& GetBody() const;
    void SetBody(Pointer<DeclarationBlock> body);
    bool HasBody() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<DeclarationBlock> myClassBody;
};

class ParameterNode : public AbstractDeclaration {
public:
    ParameterNode(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type, Pointer<IAnnotatedNode> typeNode);

    const IAnnotatedNode& GetTypeNode() const;

    const IAnnotatedNode& GetDefault() const;
    void SetDefault(Pointer<IAnnotatedNode> defaultNode);
    bool HasDefault() const;

    const AbstractType* GetType() const override;

    void RunVisitor(INodeVisitor& visitor) const override;

protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myType;
    Pointer<IAnnotatedNode> myDefault;
};

class VariableNode : public AbstractDeclaration {
public:
    explicit VariableNode(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type);

    const IAnnotatedNode& GetTypeNode() const;
    void SetTypeNode(Pointer<IAnnotatedNode> typeNode);
    bool HasTypeNode() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor & visitor) const override;

private:
    Pointer<IAnnotatedNode> myType;
};

class ParameterList : public AbstractNode {
public:
    explicit ParameterList(const Lexeme& lexeme);

    const std::vector<Pointer<ParameterNode>>& GetParameters() const;
    void AddParameter(Pointer<ParameterNode> param);

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    std::vector<Pointer<ParameterNode>> myParameters;
};

class FunctionDeclaration : public AbstractDeclaration {
public:
    FunctionDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type, Pointer<ParameterList> parameters, Pointer<IAnnotatedNode> body);

    const ParameterList& GetParameters() const;

    const IAnnotatedNode& GetBody() const;

    const IAnnotatedNode& GetReturn() const;
    void SetReturn(Pointer<IAnnotatedNode> returnNode);
    bool HasReturnNode() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;

    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<ParameterList> myParams;
    Pointer<IAnnotatedNode> myBody;
    Pointer<IAnnotatedNode> myReturn;
};

class PropertyDeclaration : public AbstractDeclaration {
public:
    PropertyDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type, const Lexeme& keyword);

    bool IsMutable() const;
    std::string GetKeyword() const;

    const IAnnotatedNode& GetTypeNode() const;
    void SetTypeNode(Pointer<IAnnotatedNode> typeNode);
    bool HasTypeNode() const;

    const IAnnotatedNode& GetInitialization() const;
    void SetInitialization(Pointer<IAnnotatedNode> initNode);
    bool HasInitialization() const;

    void RunVisitor(INodeVisitor& visitor) const override;
protected:
    std::string GetName() const override;
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    Pointer<IAnnotatedNode> myType;
    Pointer<IAnnotatedNode> myInit;
    Lexeme myKeyword;
};
