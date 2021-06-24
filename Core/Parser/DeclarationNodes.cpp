#include "DeclarationNodes.h"

std::string AbstractDeclaration::GetIdentifierName() const {
    return myIdentifier->GetLexeme().GetValue<std::string>();
}

const IdentifierNode& AbstractDeclaration::GetIdentifier() const {
    return *myIdentifier;
}

void AbstractDeclaration::SetIdentifier(Pointer<IdentifierNode> identifier) {
    myIdentifier = std::move(identifier);
}

void AbstractDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myIdentifier, depth);
}

DeclarationBlock::DeclarationBlock(std::vector<Pointer<AbstractDeclaration>> declarations) : myDeclarations(std::move(declarations)) {}

const std::vector<Pointer<AbstractDeclaration>>& DeclarationBlock::GetDeclarations() const {
    return myDeclarations;
}

void DeclarationBlock::AddDeclaration(Pointer<AbstractDeclaration> declaration) {
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

void ClassDeclaration::SetBody(Pointer<DeclarationBlock> body) {
    myClassBody = std::move(body);
}

bool ClassDeclaration::HasBody() const {
    return myClassBody != nullptr;
}

std::string ClassDeclaration::GetName() const {
    return "Class Decl";
}

void ClassDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    if (HasBody()) {
        visitor.VisitNode(*myClassBody, depth);
    }
}

const ITypedNode& ParameterNode::GetType() const {
    return *myType;
}

void ParameterNode::SetType(Pointer<ITypedNode> typeNode) {
    myType = std::move(typeNode);
}

const ITypedNode& ParameterNode::GetDefault() const {
    return *myDefault;
}

void ParameterNode::SetDefault(Pointer<ITypedNode> defaultNode) {
    myDefault = std::move(defaultNode);
}

bool ParameterNode::HasDefault() const {
    return myDefault != nullptr;
}

std::string ParameterNode::GetName() const {
    return "Parameter";
}

void ParameterNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    visitor.VisitNode(*myType, depth);
    if (HasDefault()) {
        visitor.VisitNode(*myDefault, depth);
    }
}

const ITypedNode& VariableNode::GetType() const {
    return *myType;
}

void VariableNode::SetType(Pointer<ITypedNode> typeNode) {
    myType = std::move(typeNode);
}

bool VariableNode::HasType() const {
    return myType != nullptr;
}

std::string VariableNode::GetName() const {
    return "Variable";
}

void VariableNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    if (HasType()) {
        visitor.VisitNode(*myType, depth);
    }
}

const std::vector<Pointer<ParameterNode>>& ParameterList::GetParameters() const {
    return myParameters;
}

void ParameterList::AddParameter(Pointer<ParameterNode> param) {
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

void FunctionDeclaration::SetParameters(Pointer<ParameterList> parameters) {
    myParams = std::move(parameters);
}

const ISyntaxNode& FunctionDeclaration::GetBody() const {
    return *myBody;
}

void FunctionDeclaration::SetBody(Pointer<ISyntaxNode> body) {
    myBody = std::move(body);
}

const ITypedNode& FunctionDeclaration::GetReturn() const {
    return *myReturn;
}

void FunctionDeclaration::SetReturn(Pointer<ITypedNode> returnNode) {
    myReturn = std::move(returnNode);
}

bool FunctionDeclaration::HasReturnNode() const {
    return myReturn != nullptr;
}

std::string FunctionDeclaration::GetName() const {
    return "Fun Decl";
}

void FunctionDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    visitor.VisitNode(*myParams, depth);
    if (HasReturnNode()) {
        visitor.VisitNode(*myReturn, depth);
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

const ITypedNode& PropertyDeclaration::GetType() const {
    return *myType;
}

void PropertyDeclaration::SetType(Pointer<ITypedNode> typeNode) {
    myType = std::move(typeNode);
}

bool PropertyDeclaration::HasType() const {
    return myType != nullptr;
}

const ITypedNode& PropertyDeclaration::GetInitialization() const {
    return *myInit;
}

void PropertyDeclaration::SetInitialization(Pointer<ITypedNode> initNode) {
    myInit = std::move(initNode);
}

bool PropertyDeclaration::HasInitialization() const {
    return myInit != nullptr;
}

std::string PropertyDeclaration::GetName() const {
    return "V" + GetKeyword().substr(1) + " Decl";
}

void PropertyDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    if (HasType()) {
        visitor.VisitNode(*myType, depth);
    }
    if (HasInitialization()) {
        visitor.VisitNode(*myInit, depth);
    }
}
