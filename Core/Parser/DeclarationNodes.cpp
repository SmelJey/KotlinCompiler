#include "DeclarationNodes.h"
#include "INodeVisitor.h"

std::string AbstractDeclaration::GetIdentifierName() const {
    return myIdentifier->GetLexeme().GetValue<std::string>();
}

IdentifierNode& AbstractDeclaration::GetIdentifier() const {
    return *myIdentifier;
}

void AbstractDeclaration::SetIdentifier(Pointer<IdentifierNode> identifier) {
    myIdentifier = std::move(identifier);
}

const ISymbol* AbstractDeclaration::GetSymbol() const {
    return mySymbol;
}

void AbstractDeclaration::SetSymbol(const ISymbol* symbol) {
    mySymbol = symbol;
}

void AbstractDeclaration::AcceptVisitor(INodeVisitor& visitor, int depth) const {
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

void DeclarationBlock::AcceptVisitor(INodeVisitor& visitor, int depth) const {
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

void ClassDeclaration::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    if (HasBody()) {
        visitor.VisitNode(*myClassBody, depth);
    }
}

const IAnnotatedNode& ParameterNode::GetTypeNode() const {
    return *myType;
}

void ParameterNode::SetTypeNode(Pointer<IAnnotatedNode> typeNode) {
    myType = std::move(typeNode);
}

const IAnnotatedNode& ParameterNode::GetDefault() const {
    return *myDefault;
}

void ParameterNode::SetDefault(Pointer<IAnnotatedNode> defaultNode) {
    myDefault = std::move(defaultNode);
}

bool ParameterNode::HasDefault() const {
    return myDefault != nullptr;
}

const ITypeSymbol* ParameterNode::GetType() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(GetSymbol());
    if (varSym != nullptr) {
        return varSym->GetType();
    }

    return dynamic_cast<const UnresolvedSymbol*>(GetSymbol());
}

std::string ParameterNode::GetName() const {
    return "Parameter";
}

void ParameterNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    visitor.VisitNode(*myType, depth);
    if (HasDefault()) {
        visitor.VisitNode(*myDefault, depth);
    }
}

const IAnnotatedNode& VariableNode::GetTypeNode() const {
    return *myType;
}

void VariableNode::SetTypeNode(Pointer<IAnnotatedNode> typeNode) {
    myType = std::move(typeNode);
}

bool VariableNode::HasTypeNode() const {
    return myType != nullptr;
}

std::string VariableNode::GetName() const {
    return "Variable";
}

void VariableNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    if (HasTypeNode()) {
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

void ParameterList::AcceptVisitor(INodeVisitor& visitor, int depth) const {
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

const IAnnotatedNode& FunctionDeclaration::GetBody() const {
    return *myBody;
}

void FunctionDeclaration::SetBody(Pointer<IAnnotatedNode> body) {
    myBody = std::move(body);
}

const IAnnotatedNode& FunctionDeclaration::GetReturn() const {
    return *myReturn;
}

void FunctionDeclaration::SetReturn(Pointer<IAnnotatedNode> returnNode) {
    myReturn = std::move(returnNode);
}

bool FunctionDeclaration::HasReturnNode() const {
    return myReturn != nullptr;
}

std::string FunctionDeclaration::GetName() const {
    return "Fun Decl";
}

void FunctionDeclaration::AcceptVisitor(INodeVisitor& visitor, int depth) const {
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

const IAnnotatedNode& PropertyDeclaration::GetTypeNode() const {
    return *myType;
}

void PropertyDeclaration::SetTypeNode(Pointer<IAnnotatedNode> typeNode) {
    myType = std::move(typeNode);
}

bool PropertyDeclaration::HasTypeNode() const {
    return myType != nullptr;
}

const IAnnotatedNode& PropertyDeclaration::GetInitialization() const {
    return *myInit;
}

void PropertyDeclaration::SetInitialization(Pointer<IAnnotatedNode> initNode) {
    myInit = std::move(initNode);
}

bool PropertyDeclaration::HasInitialization() const {
    return myInit != nullptr;
}

std::string PropertyDeclaration::GetName() const {
    return "V" + GetKeyword().substr(1) + " Decl";
}

void PropertyDeclaration::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    AbstractDeclaration::AcceptVisitor(visitor, depth);

    if (HasTypeNode()) {
        visitor.VisitNode(*myType, depth);
    }
    if (HasInitialization()) {
        visitor.VisitNode(*myInit, depth);
    }
}
