#include "SimpleNodes.h"
#include "Semantics/FunctionSymbol.h"
#include "NodeVisitor.h"

#include <sstream>

LexemeNode::LexemeNode(const Lexeme& lexeme) : myLexeme(lexeme) {}

Lexeme LexemeNode::GetLexeme() const {
    return myLexeme;
}

AbstractTypedNode::AbstractTypedNode(const Lexeme& lexeme, const ISymbol* symbol) : LexemeNode(lexeme), mySymbol(symbol) {}

const ISymbol* AbstractTypedNode::GetSymbol() const {
    return mySymbol;
}

IdentifierNode::IdentifierNode(const Lexeme& lexeme, const ITypeSymbol* defaultSym, const std::vector<const ISymbol*>& candidates)
    : AbstractTypedNode(lexeme, defaultSym), myCandidates(candidates), myType(defaultSym) {}

bool IdentifierNode::TryResolveVariable() {
    for (auto it : myCandidates) {
        auto varSym = dynamic_cast<const VariableSymbol*>(it);
        if (varSym != nullptr) {
            mySymbol = varSym;
            myType = varSym->GetType();
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
            myType = typeSym;
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
                myType = funcSym->GetReturnType();
                return true;
            }
        }
    }

    return false;
}

bool IdentifierNode::TryResolveArray(const std::vector<const ITypeSymbol*>& arguments) {
    for (auto it : myCandidates) {
        auto arraySym = dynamic_cast<const ArraySymbol*>(it);
        if (arraySym != nullptr && arguments.size() == 1) {
            if (*arguments[0] != IntegerSymbol()) {
                return false;
            }

            mySymbol = arraySym;
            myType = arraySym->GetType();
            return true;
        }
    }

    return false;
}

void IdentifierNode::UpdateCandidates(const std::vector<const ISymbol*>& candidates) {
    myCandidates = candidates;
}

const ITypeSymbol* IdentifierNode::GetType() const {
    return myType;
}

bool IdentifierNode::IsMutable() const {
    const VariableSymbol* symbol = dynamic_cast<const VariableSymbol*>(mySymbol);
    if (symbol != nullptr) {
        return symbol->IsMutable();
    }

    return false;
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

ErrorNode::ErrorNode(const Lexeme& lexeme, const UnresolvedSymbol* type, const std::string& error)
    : LexemeNode(lexeme), myError(error), myType(type) {}

const ISymbol* ErrorNode::GetSymbol() const {
    return myType;
}

std::string ErrorNode::GetName() const {
    std::stringstream ss;
    ss << "Error " << myLexeme.GetRow() << ":" << myLexeme.GetColumn() << " :: " + myError;
    return ss.str();
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

const ITypedNode* ReturnNode::GetExpression() const {
    return myExpression.get();
}

void ReturnNode::SetExpression(Pointer<ITypedNode> expression) {
    myExpression = std::move(expression);
}

bool ReturnNode::HasExpression() const {
    return myExpression != nullptr;
}

std::string ReturnNode::GetName() const {
    return "Return";
}

void ReturnNode::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    if (HasExpression()) {
        visitor.VisitNode(*myExpression, depth);
    }
}
