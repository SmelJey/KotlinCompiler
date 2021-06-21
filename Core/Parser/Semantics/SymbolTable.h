#pragma once
#include <functional>
#include <set>

#include "Symbols.h"


class SymbolTable {
public:
    SymbolTable(const SymbolTable* parent = nullptr);

    bool Contains(const ISymbol& symbol) const;
    void Add(Pointer<ISymbol> symbol);

private:
    const SymbolTable* myParentTable;
    std::set<Pointer<ISymbol>, std::less<>> mySymbols;
};
