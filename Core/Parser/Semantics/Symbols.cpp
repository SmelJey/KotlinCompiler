#include "Symbols.h"

#include "../INodeVisitor.h"
#include "SymbolTable.h"

bool ISymbol::operator<(const ISymbol& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return GetName() < rhs.GetName();
    }

    return typeid(*this).before(typeid(rhs));
}

std::string ISymbol::ToString() const {
    return GetName();
}

bool operator!=(const ISymbol& lhs, const ISymbol& rhs) {
    return lhs < rhs || rhs < lhs;
}

bool operator==(const ISymbol& lhs, const ISymbol& rhs) {
    return !(lhs != rhs);
}

bool operator<(const Pointer<ISymbol>& lhs, const Pointer<ISymbol>& rhs) {
    return *lhs < *rhs;
}

bool operator<(const ISymbol& lhs, const Pointer<ISymbol>& rhs) {
    return lhs < *rhs;
}

bool operator<(const Pointer<ISymbol>& lhs, const ISymbol& rhs) {
    return *lhs < rhs;
}

ITypeSymbol::ITypeSymbol(const std::string& name, SymbolTable* parentTable)
    : myParentTable(parentTable), myName(name), myTable(std::make_unique<SymbolTable>(parentTable)) {}

ITypeSymbol::ITypeSymbol(const std::string& name, Pointer<SymbolTable> symTable)
    : myParentTable(symTable->GetParent()), myName(name), myTable(std::move(symTable))  {}

bool ITypeSymbol::IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const {
    return assignOperation == LexemeType::OpAssign && *this == *rightOperand;
}

SymbolTable* ITypeSymbol::GetTable() const {
    return myTable.get();
}

std::string ITypeSymbol::GetName() const {
    return myName;
}

void ITypeSymbol::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myTable, depth);
}

SymbolTable* ITypeSymbol::GetParentTable() const {
    return myParentTable;
}

VariableSymbol::VariableSymbol(const std::string& name, const ITypeSymbol* type, bool isMutable)
    : myName(name), myType(type), myMutability(isMutable) {}

std::string VariableSymbol::GetName() const {
    return myName;
}

const ITypeSymbol* VariableSymbol::GetType() const {
    return myType;
}

std::string VariableSymbol::ToString() const {
    return (myMutability ? "Var " : "Val ") + GetName();
}

bool VariableSymbol::IsMutable() const {
    return myMutability;
}

void VariableSymbol::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myType, depth);
}
