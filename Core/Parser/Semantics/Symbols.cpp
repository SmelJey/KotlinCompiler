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

AbstractType::AbstractType(const std::string& name, SymbolTable* parentTable)
    : myParentTable(parentTable), myName(name), myTable(std::make_unique<SymbolTable>(parentTable)) {}

AbstractType::AbstractType(const std::string& name, Pointer<SymbolTable> symTable)
    : myParentTable(symTable->GetParent()), myName(name), myTable(std::move(symTable))  {}

bool AbstractType::IsAssignable(LexemeType assignOperation, const AbstractType* rightOperand) const {
    return assignOperation == LexemeType::OpAssign && *this == *rightOperand;
}

SymbolTable* AbstractType::GetTable() const {
    return myTable.get();
}

std::string AbstractType::GetName() const {
    return myName;
}

void AbstractType::AcceptVisitor(INodeVisitor& visitor) const {
    if (!myTable->IsEmpty()) {
        myTable->RunVisitor(visitor);
    }
}

SymbolTable* AbstractType::GetParentTable() const {
    return myParentTable;
}

VariableSymbol::VariableSymbol(const std::string& name, const AbstractType* type, bool isMutable)
    : myName(name), myType(type), myMutability(isMutable) {}

std::string VariableSymbol::GetName() const {
    return myName;
}

const AbstractType* VariableSymbol::GetType() const {
    return myType;
}

std::string VariableSymbol::ToString() const {
    return (myMutability ? "Var " : "Val ") + GetName() + " : " + myType->GetName();
}

bool VariableSymbol::IsMutable() const {
    return myMutability;
}
