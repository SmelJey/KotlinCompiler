#include "ClassSymbol.h"

#include "../INodeVisitor.h"

ClassSymbol::ClassSymbol(const std::string& name, Pointer<SymbolTable> table) : myName(name), myTable(std::move(table)) {}

SymbolTable* ClassSymbol::GetTable() const {
    return myTable.get();
}

std::string ClassSymbol::GetName() const {
    return myName;
}

Pointer<ITypeSymbol> ClassSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> ClassSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

std::string ClassSymbol::ToString() const {
    return "Class " + GetName();
}

void ClassSymbol::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myTable, depth);
}

