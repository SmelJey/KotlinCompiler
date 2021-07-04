#pragma once
#include "Symbols.h"
#include "SymbolTable.h"


class ClassSymbol : public AbstractType {
public:
    ClassSymbol(const std::string& name, Pointer<SymbolTable> table);

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;

    std::string ToString() const override;
};
