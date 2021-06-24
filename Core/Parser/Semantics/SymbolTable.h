#pragma once
#include <functional>
#include <set>
#include <map>

#include "Symbols.h"


class ClassSymbol;
class FunctionSymbol;

class SymbolTable {
public:
    SymbolTable(const SymbolTable* parent = nullptr);

    std::vector<const ISymbol*> GetSymbols(const std::string& name) const;

    const ITypeSymbol* GetType(const std::string& name) const;
    const ITypeSymbol* GetClass(const std::string& name) const;
    std::vector<const FunctionSymbol*> GetFunctions(const std::string& name) const;

    bool Contains(const ISymbol& symbol) const;
    bool Add(Pointer<ISymbol> symbol);

    const UnresolvedSymbol* GetUnresolvedSymbol() const;
    const UnitTypeSymbol* GetUnitSymbol() const;

private:
    const SymbolTable* myParentTable;
    Pointer<UnresolvedSymbol> myUnresolved;
    Pointer<UnitTypeSymbol> myUnitSymbol;
    std::map<std::string, std::vector<Pointer<ISymbol>>> mySymbols;
    //std::set<Pointer<ISymbol>, std::less<>> mySymbols;
};
