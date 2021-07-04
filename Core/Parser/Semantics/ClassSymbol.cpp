#include "ClassSymbol.h"

#include "FundamentalType.h"

ClassSymbol::ClassSymbol(const std::string& name, Pointer<SymbolTable> table) : AbstractType(name, std::move(table)) {}

Pointer<AbstractType> ClassSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<AbstractType> ClassSymbol::IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

std::string ClassSymbol::ToString() const {
    return "Class " + GetName();
}


