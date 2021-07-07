#include "DeclarationNodes.h"
#include "INodeVisitor.h"

AbstractDeclaration::AbstractDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type)
    : AbstractNode(identifier->GetLexeme()), myUnitSym(type), myIdentifier(std::move(identifier)) {}

std::string AbstractDeclaration::GetIdentifierName() const {
    return myIdentifier->GetLexeme().GetValue<std::string>();
}

IdentifierNode& AbstractDeclaration::GetIdentifier() const {
    return *myIdentifier;
}

const ISymbol* AbstractDeclaration::GetSymbol() const {
    return mySymbol;
}

void AbstractDeclaration::SetSymbol(const ISymbol* symbol) {
    mySymbol = symbol;
    myIdentifier->Resolve(symbol);
}

const AbstractType* AbstractDeclaration::GetType() const {
    return myUnitSym;
}

Lexeme AbstractDeclaration::GetLexeme() const {
    return myIdentifier->GetLexeme();
}

void AbstractDeclaration::PropagateVisitor(INodeVisitor& visitor) const {
    myIdentifier->RunVisitor(visitor);
}

DeclarationBlock::DeclarationBlock(const Lexeme& lexeme, std::vector<Pointer<AbstractDeclaration>> declarations)
    : AbstractNode(lexeme.CopyEmptyOfType(LexemeType::Ignored)), myDeclarations(std::move(declarations)) {}

const std::vector<Pointer<AbstractDeclaration>>& DeclarationBlock::GetDeclarations() const {
    return myDeclarations;
}

void DeclarationBlock::AddDeclaration(Pointer<AbstractDeclaration> declaration) {
    myDeclarations.push_back(std::move(declaration));
}

void DeclarationBlock::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string DeclarationBlock::GetName() const {
    return "Decl Block";
}

void DeclarationBlock::PropagateVisitor(INodeVisitor& visitor) const {
    for (auto& declaration : myDeclarations) {
        declaration->RunVisitor(visitor);
    }
}

ClassDeclaration::ClassDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type) : AbstractDeclaration(std::move(identifier), type) {}

const DeclarationBlock& ClassDeclaration::GetBody() const {
    return *myClassBody;
}

void ClassDeclaration::SetBody(Pointer<DeclarationBlock> body) {
    myClassBody = std::move(body);
}

bool ClassDeclaration::HasBody() const {
    return myClassBody != nullptr;
}

void ClassDeclaration::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string ClassDeclaration::GetName() const {
    return "Class Decl";
}

void ClassDeclaration::PropagateVisitor(INodeVisitor& visitor) const {
    AbstractDeclaration::PropagateVisitor(visitor);

    if (HasBody()) {
        myClassBody->RunVisitor(visitor);
    }
}

ParameterNode::ParameterNode(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type, Pointer<IAnnotatedNode> typeNode)
    : AbstractDeclaration(std::move(identifier), type), myType(std::move(typeNode)) {}

const IAnnotatedNode& ParameterNode::GetTypeNode() const {
    return *myType;
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

const AbstractType* ParameterNode::GetType() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(GetSymbol());
    if (varSym != nullptr) {
        return varSym->GetType();
    }

    return dynamic_cast<const UnresolvedSymbol*>(GetSymbol());
}

void ParameterNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string ParameterNode::GetName() const {
    return "Parameter";
}

void ParameterNode::PropagateVisitor(INodeVisitor& visitor) const {
    AbstractDeclaration::PropagateVisitor(visitor);

    myType->RunVisitor(visitor);
    if (HasDefault()) {
        myDefault->RunVisitor(visitor);
    }
}

VariableNode::VariableNode(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type) : AbstractDeclaration(std::move(identifier), type) {}

const IAnnotatedNode& VariableNode::GetTypeNode() const {
    return *myType;
}

void VariableNode::SetTypeNode(Pointer<IAnnotatedNode> typeNode) {
    myType = std::move(typeNode);
}

bool VariableNode::HasTypeNode() const {
    return myType != nullptr;
}

void VariableNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string VariableNode::GetName() const {
    return "Variable";
}

void VariableNode::PropagateVisitor(INodeVisitor& visitor) const {
    AbstractDeclaration::PropagateVisitor(visitor);

    if (HasTypeNode()) {
        myType->RunVisitor(visitor);
    }
}

ParameterList::ParameterList(const Lexeme& lexeme) : AbstractNode(lexeme) {}

const std::vector<Pointer<ParameterNode>>& ParameterList::GetParameters() const {
    return myParameters;
}

void ParameterList::AddParameter(Pointer<ParameterNode> param) {
    myParameters.push_back(std::move(param));
}

void ParameterList::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string ParameterList::GetName() const {
    return "Params";
}

void ParameterList::PropagateVisitor(INodeVisitor& visitor) const {
    for (auto& param : myParameters) {
        param->RunVisitor(visitor);
    }
}

FunctionDeclaration::FunctionDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type, Pointer<ParameterList> parameters)
    : AbstractDeclaration(std::move(identifier), type), myParams(std::move(parameters)) {}

const ParameterList& FunctionDeclaration::GetParameters() const {
    return *myParams;
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

void FunctionDeclaration::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string FunctionDeclaration::GetName() const {
    return "Fun Decl";
}

void FunctionDeclaration::PropagateVisitor(INodeVisitor& visitor) const {
    AbstractDeclaration::PropagateVisitor(visitor);

    myParams->RunVisitor(visitor);
    if (HasReturnNode()) {
        myReturn->RunVisitor(visitor);
    }

    myBody->RunVisitor(visitor);
}

PropertyDeclaration::PropertyDeclaration(Pointer<IdentifierNode> identifier, const UnitTypeSymbol* type, const Lexeme& keyword)
    : AbstractDeclaration(std::move(identifier), type), myKeyword(keyword) {}

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

void PropertyDeclaration::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string PropertyDeclaration::GetName() const {
    return "V" + GetKeyword().substr(1) + " Decl";
}

void PropertyDeclaration::PropagateVisitor(INodeVisitor& visitor) const {
    AbstractDeclaration::PropagateVisitor(visitor);

    if (HasTypeNode()) {
        myType->RunVisitor(visitor);
    }
    if (HasInitialization()) {
        myInit->RunVisitor(visitor);
    }
}
