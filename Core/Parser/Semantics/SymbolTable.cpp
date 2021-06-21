#include "SymbolTable.h"

SymbolTable::SymbolTable(const SymbolTable* parent) : myParentTable(parent) {}

bool SymbolTable::Contains(const ISymbol& symbol) const {
    if (!mySymbols.count(symbol)) {
        if (myParentTable == nullptr) {
            return false;
        }

        return myParentTable->Contains(symbol);
    }

    return true;
}

void SymbolTable::Add(Pointer<ISymbol> symbol) {
    mySymbols.insert(std::move(symbol));
}
