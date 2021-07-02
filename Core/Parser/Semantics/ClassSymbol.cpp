#include "ClassSymbol.h"

#include "BuiltInTypes.h"

ClassSymbol::ClassSymbol(const std::string& name, Pointer<SymbolTable> table) : ITypeSymbol(name, std::move(table)) {}

Pointer<ITypeSymbol> ClassSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> ClassSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

std::string ClassSymbol::ToString() const {
    return "Class " + GetName();
}


