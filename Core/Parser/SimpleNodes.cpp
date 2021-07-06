#include "SimpleNodes.h"
#include "Semantics/FunctionSymbol.h"
#include "INodeVisitor.h"
#include "ExpressionNodes.h"

#include <sstream>

AbstractTypedNode::AbstractTypedNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractNode(lexeme), mySymbol(symbol) {}

const ISymbol* AbstractTypedNode::GetSymbol() const {
    return mySymbol;
}

const AbstractType* AbstractTypedNode::GetType() const {
    return dynamic_cast<const AbstractType*>(GetSymbol());
}

UnitTypedNode::UnitTypedNode(const Lexeme& lexeme, const UnitTypeSymbol* symbol) : AbstractNode(lexeme), myType(symbol) {}

const ISymbol* UnitTypedNode::GetSymbol() const {
    return myType;
}

const AbstractType* UnitTypedNode::GetType() const {
    return myType;
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme, const AbstractType* defaultSym, const std::vector<const ISymbol*>& candidates)
    : AbstractTypedNode(lexeme, defaultSym), myCandidates(candidates) {}

std::string IdentifierNode::GetIdentifier() const {
    return GetLexeme().GetValue<std::string>();
}

bool IdentifierNode::TryResolveVariable() {
    for (auto it : myCandidates) {
        auto varSym = dynamic_cast<const VariableSymbol*>(it);
        if (varSym != nullptr) {
            mySymbol = varSym;
            return true;
        }
    }

    return false;
}

bool IdentifierNode::TryResolveType() {
    for (auto it : myCandidates) {
        auto typeSym = dynamic_cast<const AbstractType*>(it);
        if (typeSym != nullptr) {
            mySymbol = typeSym;
            return true;
        }
    }

    return false;
}

bool IdentifierNode::TryResolveFunc(const std::vector<const AbstractType*>& arguments) {
    for (auto it : myCandidates) {
        auto funcSym = dynamic_cast<const FunctionSymbol*>(it);
        if (funcSym != nullptr && funcSym->GetParametersCount() == arguments.size()) {
            bool isResolved = true;
            for (int i = 0; isResolved && i < arguments.size(); i++) {
                isResolved = *funcSym->GetParameter(i) == *arguments[i];
            }

            if (isResolved) {
                mySymbol = funcSym;
                return true;
            }
        }
    }

    return false;
}

void IdentifierNode::Resolve(const ISymbol* symbol) {
    mySymbol = symbol;
}

const ISymbol* IdentifierNode::GetSymbol() const {
    return mySymbol;
}

const AbstractType* IdentifierNode::GetType() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(mySymbol);
    if (varSym != nullptr) {
        return varSym->GetType();
    }

    auto funcSym = dynamic_cast<const FunctionSymbol*>(mySymbol);
    if (funcSym != nullptr) {
        return funcSym->GetReturnType();
    }

    return dynamic_cast<const AbstractType*>(mySymbol);
}

bool IdentifierNode::IsAssignable() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(mySymbol);
    if (varSym == nullptr) {
        return false;
    }

    return varSym->IsMutable();
}

void IdentifierNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string IdentifierNode::GetName() const {
    return "Identifier :: " + GetLexeme().GetValue<std::string>();
}

IntegerNode::IntegerNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

void IntegerNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string IntegerNode::GetName() const {
    return "Integer :: " + std::to_string(GetLexeme().GetValue<uint64_t>());
}

DoubleNode::DoubleNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

void DoubleNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string DoubleNode::GetName() const {
    return "Real :: " + std::to_string(GetLexeme().GetValue<double>());
}

BooleanNode::BooleanNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

void BooleanNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string BooleanNode::GetName() const {
    return "Boolean :: " + GetLexeme().GetValue<std::string>();
}

StringNode::StringNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

void StringNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string StringNode::GetName() const {
    return "String :: " + GetLexeme().GetValue<std::string>();
}

TypeNode::TypeNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}


const TypeArgumentsNode* TypeNode::GetTypeArgs() const {
    return myTypeArgs.get();
}

void TypeNode::SetTypeArgs(Pointer<TypeArgumentsNode> args) {
    myTypeArgs = std::move(args);
}

bool TypeNode::HasTypeArgs() const {
    return myTypeArgs != nullptr;
}

void TypeNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

void TypeNode::PropagateVisitor(INodeVisitor& visitor) const {
    AbstractTypedNode::PropagateVisitor(visitor);
    if (HasTypeArgs()) {
        myTypeArgs->RunVisitor(visitor);
    }
}

std::string TypeNode::GetName() const {
    return "Type :: " + GetLexeme().GetValue<std::string>();
}

BreakNode::BreakNode(const Lexeme& lexeme, const UnitTypeSymbol* type) : UnitTypedNode(lexeme, type) {}

void BreakNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string BreakNode::GetName() const {
    return "Break";
}

ContinueNode::ContinueNode(const Lexeme& lexeme, const UnitTypeSymbol* type) : UnitTypedNode(lexeme, type) {}

void ContinueNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string ContinueNode::GetName() const {
    return "Continue";
}

ReturnNode::ReturnNode(const Lexeme& lexeme, const UnitTypeSymbol* type) : UnitTypedNode(lexeme, type) {}

const IAnnotatedNode* ReturnNode::GetExpression() const {
    return myExpression.get();
}

void ReturnNode::SetExpression(Pointer<IAnnotatedNode> expression) {
    myExpression = std::move(expression);
}

bool ReturnNode::HasExpression() const {
    return myExpression != nullptr;
}

const ISymbol* ReturnNode::GetSymbol() const {
    return GetType();
}

const AbstractType* ReturnNode::GetType() const {
    if (HasExpression()) {
        return myExpression->GetType();
    }

    return UnitTypedNode::GetType();
}

void ReturnNode::RunVisitor(INodeVisitor& visitor) const {
    visitor.EnterNode(*this);
    IVisitable::RunVisitor(visitor);
    visitor.ExitNode(*this);
}

std::string ReturnNode::GetName() const {
    return "Return";
}

void ReturnNode::PropagateVisitor(INodeVisitor& visitor) const {
    if (HasExpression()) {
        myExpression->RunVisitor(visitor);
    }
}
