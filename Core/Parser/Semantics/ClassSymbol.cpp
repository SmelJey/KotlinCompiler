#include "ClassSymbol.h"

ClassSymbol::ClassSymbol(const std::string& name, Pointer<SymbolTable> table) : myName(name), myTable(std::move(table)) {}

const SymbolTable& ClassSymbol::GetTable() const {
    return *myTable;
}

std::string ClassSymbol::GetName() const {
    return "Class " + myName;
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

