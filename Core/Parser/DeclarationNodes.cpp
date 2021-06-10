#include "DeclarationNodes.h"

std::string IDeclaration::GetIdentifier() const {
    return myIdentifier->GetLexeme().GetValue<std::string>();
}

const IdentifierNode& IDeclaration::GetIdentifierNode() const {
    return *myIdentifier;
}

void IDeclaration::SetIdentifier(std::unique_ptr<IdentifierNode> identifier) {
    myIdentifier = std::move(identifier);
}

void IDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myIdentifier, depth);
}

DeclarationBlock::DeclarationBlock(std::vector<std::unique_ptr<IDeclaration>> declarations) : myDeclarations(std::move(declarations)) {}

const std::vector<std::unique_ptr<IDeclaration>>& DeclarationBlock::GetDeclarations() const {
    return myDeclarations;
}

void DeclarationBlock::AddDeclaration(std::unique_ptr<IDeclaration> declaration) {
    myDeclarations.push_back(std::move(declaration));
}

std::string DeclarationBlock::GetName() const {
    return "Decl Block";
}

void DeclarationBlock::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    for (auto& declaration : myDeclarations) {
        visitor.VisitNode(*declaration, depth);
    }
}

const DeclarationBlock& ClassDeclaration::GetBody() const {
    return *myClassBody;
}

void ClassDeclaration::SetBody(std::unique_ptr<DeclarationBlock> body) {
    myClassBody = std::move(body);
}

bool ClassDeclaration::HasBody() const {
    return myClassBody != nullptr;
}

std::string ClassDeclaration::GetName() const {
    return "Class Decl";
}

void ClassDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    IDeclaration::AcceptVisitor(visitor, depth);

    if (HasBody()) {
        visitor.VisitNode(*myClassBody, depth);
    }
}

const IdentifierNode& ParameterNode::GetIdentifier() const {
    return *myIdentifierNode;
}

void ParameterNode::SetIdentifier(std::unique_ptr<IdentifierNode> identifier) {
    myIdentifierNode = std::move(identifier);
}

const ISyntaxNode& ParameterNode::GetTypeNode() const {
    return *myTypeNode;
}

void ParameterNode::SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode) {
    myTypeNode = std::move(typeNode);
}

const ISyntaxNode& ParameterNode::GetDefaultNode() const {
    return *myDefaultNode;
}

void ParameterNode::SetDefaultNode(std::unique_ptr<ISyntaxNode> defaultNode) {
    myDefaultNode = std::move(defaultNode);
}

bool ParameterNode::HasDefaultNode() const {
    return myDefaultNode != nullptr;
}

std::string ParameterNode::GetName() const {
    return "Parameter";
}

void ParameterNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myIdentifierNode, depth);
    visitor.VisitNode(*myTypeNode, depth);
    if (HasDefaultNode()) {
        visitor.VisitNode(*myDefaultNode, depth);
    }
}

const IdentifierNode& VariableNode::GetIdentifier() const {
    return *myIdentifierNode;
}

void VariableNode::SetIdentifier(std::unique_ptr<IdentifierNode> identifier) {
    myIdentifierNode = std::move(identifier);
}

const ISyntaxNode& VariableNode::GetTypeNode() const {
    return *myTypeNode;
}

void VariableNode::SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode) {
    myTypeNode = std::move(typeNode);
}

bool VariableNode::HasTypeNode() const {
    return myTypeNode != nullptr;
}

std::string VariableNode::GetName() const {
    return "Variable";
}

void VariableNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myIdentifierNode, depth);
    if (HasTypeNode()) {
        visitor.VisitNode(*myTypeNode, depth);
    }
}

const std::vector<std::unique_ptr<ParameterNode>>& ParameterList::GetParameters() const {
    return myParameters;
}

void ParameterList::AddParameter(std::unique_ptr<ParameterNode> param) {
    myParameters.push_back(std::move(param));
}

std::string ParameterList::GetName() const {
    return "Params";
}

void ParameterList::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    for (auto& param : myParameters) {
        visitor.VisitNode(*param, depth);
    }
}

const ParameterList& FunctionDeclaration::GetParameters() const {
    return *myParams;
}

void FunctionDeclaration::SetParameters(std::unique_ptr<ParameterList> parameters) {
    myParams = std::move(parameters);
}

const ISyntaxNode& FunctionDeclaration::GetBody() const {
    return *myBody;
}

void FunctionDeclaration::SetBody(std::unique_ptr<ISyntaxNode> body) {
    myBody = std::move(body);
}

const ISyntaxNode& FunctionDeclaration::GetReturnNode() const {
    return *myReturnNode;
}

void FunctionDeclaration::SetReturnNode(std::unique_ptr<ISyntaxNode> returnNode) {
    myReturnNode = std::move(returnNode);
}

bool FunctionDeclaration::HasReturnNode() const {
    return myReturnNode != nullptr;
}

std::string FunctionDeclaration::GetName() const {
    return "Fun Decl";
}

void FunctionDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    IDeclaration::AcceptVisitor(visitor, depth);

    visitor.VisitNode(*myParams, depth);
    if (HasReturnNode()) {
        visitor.VisitNode(*myReturnNode, depth);
    }
    
    visitor.VisitNode(*myBody, depth);
}

PropertyDeclaration::PropertyDeclaration(const Lexeme& keyword) : myKeyword(keyword) {}

bool PropertyDeclaration::IsMutable() const {
    return GetKeyword() == "var";
}

std::string PropertyDeclaration::GetKeyword() const {
    return myKeyword.GetValue<std::string>();
}

const ISyntaxNode& PropertyDeclaration::GetType() const {
    return *myTypeNode;
}

void PropertyDeclaration::SetType(std::unique_ptr<ISyntaxNode> typeNode) {
    myTypeNode = std::move(typeNode);
}

bool PropertyDeclaration::HasType() const {
    return myTypeNode != nullptr;
}

const ISyntaxNode& PropertyDeclaration::GetInitialization() const {
    return *myInit;
}

void PropertyDeclaration::SetInitialization(std::unique_ptr<ISyntaxNode> initNode) {
    myInit = std::move(initNode);
}

bool PropertyDeclaration::HasInitialization() const {
    return myInit != nullptr;
}

std::string PropertyDeclaration::GetName() const {
    return "V" + GetKeyword().substr(1) + " Decl";
}

void PropertyDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    IDeclaration::AcceptVisitor(visitor, depth);

    if (HasType()) {
        visitor.VisitNode(*myTypeNode, depth);
    }
    if (HasInitialization()) {
        visitor.VisitNode(*myInit, depth);
    }
}
