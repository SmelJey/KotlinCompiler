#include "DeclarationNodes.h"

IDeclaration::IDeclaration(const Lexeme& identifierLexeme) : ILexemeNode(identifierLexeme) {}

std::string IDeclaration::GetIdentifier() const {
    return myLexeme.GetValue<std::string>();
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

ClassDeclaration::ClassDeclaration(const Lexeme& lexeme, std::unique_ptr<DeclarationBlock> body) : IDeclaration(lexeme), myClassBody(std::move(body)) {}

const DeclarationBlock& ClassDeclaration::GetClassBody() const {
    return *myClassBody;
}

std::string ClassDeclaration::GetName() const {
    return "Class Decl :: " + GetIdentifier();
}

void ClassDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myClassBody, depth);
}

Variable::Variable(const Lexeme& parameterLexeme) : ILexemeNode(parameterLexeme) {}

const ISyntaxNode& Variable::GetTypeNode() const {
    return *myTypeNode;
}

void Variable::SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode) {
    myTypeNode = std::move(typeNode);
}

const ISyntaxNode& Variable::GetDefaultNode() const {
    return *myDefaultNode;
}

void Variable::SetDefaultNode(std::unique_ptr<ISyntaxNode> defaultNode) {
    myDefaultNode = std::move(defaultNode);
}

bool Variable::HasDefaultNode() const {
    return myDefaultNode != nullptr;
}

std::string Variable::GetName() const {
    return "Param :: " + myLexeme.GetValue<std::string>();
}

void Variable::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myTypeNode, depth);
    if (HasDefaultNode()) {
        visitor.VisitNode(*myDefaultNode, depth);
    }
}

const std::vector<std::unique_ptr<Variable>>& ParameterList::GetParameters() const {
    return myParameters;
}

void ParameterList::AddParameter(std::unique_ptr<Variable> param) {
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

FunctionDeclaration::FunctionDeclaration(const Lexeme& lexeme)
    : IDeclaration(lexeme) {}

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
    return "Fun Decl :: " + GetIdentifier();
}

void FunctionDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myParams, depth);
    if (HasReturnNode()) {
        visitor.VisitNode(*myReturnNode, depth);
    }
    
    visitor.VisitNode(*myBody, depth);
}

PropertyDeclaration::PropertyDeclaration(const Lexeme& lexeme, const Lexeme& keyword) : IDeclaration(lexeme), myKeyword(keyword) {}

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
    return "V" + GetKeyword().substr(1) + " Decl :: " + myLexeme.GetValue<std::string>();
}

void PropertyDeclaration::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    if (HasType()) {
        visitor.VisitNode(*myTypeNode, depth);
    }
    if (HasInitialization()) {
        visitor.VisitNode(*myInit, depth);
    }
}
