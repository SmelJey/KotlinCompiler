#pragma once
#include "Symbols.h"
#include "SymbolTable.h"


class ClassSymbol : public ISymbol {
public:
    ClassSymbol(const std::string& name, Pointer<SymbolTable> table);

    std::string GetName() const override;

private:
    std::string myName;

    Pointer<SymbolTable> myTable;

};
