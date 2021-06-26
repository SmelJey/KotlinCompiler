#include "SimpleNodes.h"
#include "Semantics/FunctionSymbol.h"
#include "INodeVisitor.h"

#include <sstream>

LexemeNode::LexemeNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

Lexeme LexemeNode::GetLexeme() const {
    return myLexeme;
}

AbstractTypedNode::AbstractTypedNode(const Lexeme& lexeme, const ISymbol* symbol) : LexemeNode(lexeme), mySymbol(symbol) {}

const ISymbol* AbstractTypedNode::GetSymbol() const {
    return mySymbol;
}

const ITypeSymbol* AbstractTypedNode::GetType() const {
    return dynamic_cast<const ITypeSymbol*>(GetSymbol());
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme, const ITypeSymbol* defaultSym, const std::vector<const ISymbol*>& candidates)
    : AbstractTypedNode(lexeme, defaultSym), myCandidates(candidates) {}

std::string IdentifierNode::GetIdentifier() const {
    return myLexeme.GetValue<std::string>();
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
        auto typeSym = dynamic_cast<const ITypeSymbol*>(it);
        if (typeSym != nullptr) {
            mySymbol = typeSym;
            return true;
        }
    }

    return false;
}

bool IdentifierNode::TryResolveFunc(const std::vector<const ITypeSymbol*>& arguments) {
    for (auto it : myCandidates) {
        auto funcSym = dynamic_cast<const FunctionSymbol*>(it);
        if (funcSym != nullptr && funcSym->GetParametersCount() == arguments.size()) {
            bool isResolved = true;
            for (int i = 0; i < arguments.size(); i++) {
                if (funcSym->GetParameter(i) != *arguments[i]) {
                    isResolved = false;
                    break;
                }
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

const ITypeSymbol* IdentifierNode::GetType() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(mySymbol);
    if (varSym != nullptr) {
        return varSym->GetType();
    }

    auto funcSym = dynamic_cast<const FunctionSymbol*>(mySymbol);
    if (funcSym != nullptr) {
        return funcSym->GetReturnType();
    }

    return dynamic_cast<const ITypeSymbol*>(mySymbol);
}

bool IdentifierNode::IsAssignable() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(mySymbol);
    if (varSym == nullptr) {
        return false;
    }

    return varSym->IsMutable();
}

std::string IdentifierNode::GetName() const {
    return "Identifier :: " + myLexeme.GetValue<std::string>();
}

IntegerNode::IntegerNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

std::string IntegerNode::GetName() const {
    return "Integer :: " + std::to_string(myLexeme.GetValue<uint64_t>());
}

DoubleNode::DoubleNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

std::string DoubleNode::GetName() const {
    return "Real :: " + std::to_string(myLexeme.GetValue<double>());
}

BooleanNode::BooleanNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

std::string BooleanNode::GetName() const {
    return "Boolean :: " + myLexeme.GetValue<std::string>();
}

StringNode::StringNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

std::string StringNode::GetName() const {
    return "String :: " + myLexeme.GetValue<std::string>();
}

TypeNode::TypeNode(const Lexeme& lexeme, const ISymbol* symbol) : AbstractTypedNode(lexeme, symbol) {}

std::string TypeNode::GetName() const {
    return "Type :: " + myLexeme.GetValue<std::string>();
}

BreakNode::BreakNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string BreakNode::GetName() const {
    return "Break";
}

ContinueNode::ContinueNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

std::string ContinueNode::GetName() const {
    return "Continue";
}

ReturnNode::ReturnNode(const Lexeme& lexeme) : LexemeNode(lexeme) {}

const IAnnotatedNode* ReturnNode::GetExpression() const {
    return myExpression.get();
}

void ReturnNode::SetExpression(Pointer<IAnnotatedNode> expression) {
    myExpression = std::move(expression);
}

bool ReturnNode::HasExpression() const {
    return myExpression != nullptr;
}

std::string ReturnNode::GetName() const {
    return "Return";
}

void ReturnNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    if (HasExpression()) {
        visitor.VisitNode(*myExpression, depth);
    }
}
