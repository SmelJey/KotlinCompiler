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

Parameter::Parameter(const Lexeme& parameterLexeme) : ILexemeNode(parameterLexeme) {}

const ISyntaxNode& Parameter::GetTypeNode() const {
    return *myTypeNode;
}

void Parameter::SetTypeNode(std::unique_ptr<ISyntaxNode> typeNode) {
    myTypeNode = std::move(typeNode);
}

const ISyntaxNode& Parameter::GetDefaultNode() const {
    return *myDefaultNode;
}

void Parameter::SetDefaultNode(std::unique_ptr<ISyntaxNode> defaultNode) {
    myDefaultNode = std::move(defaultNode);
}

bool Parameter::HasDefaultNode() const {
    return myDefaultNode != nullptr;
}

std::string Parameter::GetName() const {
    return "Param :: " + myLexeme.GetValue<std::string>();
}

void Parameter::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myTypeNode, depth);
    if (HasDefaultNode()) {
        visitor.VisitNode(*myDefaultNode, depth);
    }
}

const std::vector<std::unique_ptr<Parameter>>& ParameterList::GetParameters() const {
    return myParameters;
}

void ParameterList::AddParameter(std::unique_ptr<Parameter> param) {
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

const std::vector<std::unique_ptr<ISyntaxNode>>& BlockNode::GetStatements() const {
    return myStatements;
}

void BlockNode::AddStatement(std::unique_ptr<ISyntaxNode> statement) {
    myStatements.push_back(std::move(statement));
}

std::string BlockNode::GetName() const {
    return "Block";
}

void BlockNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    for (auto& statement : myStatements) {
        visitor.VisitNode(*statement, depth);
    }
}
