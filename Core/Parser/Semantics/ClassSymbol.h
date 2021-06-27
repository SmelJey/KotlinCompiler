#pragma once
#include "Symbols.h"
#include "SymbolTable.h"


class ClassSymbol : public ITypeSymbol {
public:
    ClassSymbol(const std::string& name, Pointer<SymbolTable> table);

    SymbolTable* GetTable() const;
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;

    std::string ToString() const override;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::string myName;

    Pointer<SymbolTable> myTable;
};
