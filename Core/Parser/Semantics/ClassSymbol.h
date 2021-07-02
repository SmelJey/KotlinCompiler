#pragma once
#include "Symbols.h"
#include "SymbolTable.h"


class ClassSymbol : public ITypeSymbol {
public:
    ClassSymbol(const std::string& name, Pointer<SymbolTable> table);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;

    std::string ToString() const override;
};
