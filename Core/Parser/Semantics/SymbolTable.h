#pragma once
#include <map>

#include "Symbols.h"


class ClassSymbol;
class FunctionSymbol;

class SymbolTable : public IVisitable {
public:
    SymbolTable(SymbolTable* parent = nullptr);
    SymbolTable(SymbolTable& src) = delete;

    SymbolTable* GetParent() const;
    std::vector<const ISymbol*> GetSymbols(const std::string& name) const;

    const ITypeSymbol* GetType(const std::string& name) const;
    const ISymbol* GetVariable(const std::string& name) const;
    const ITypeSymbol* GetClass(const std::string& name) const;
    std::vector<const FunctionSymbol*> GetFunctions(const std::string& name) const;

    const ISymbol* GetFunction(const std::string& name, const std::vector<const ITypeSymbol*>& params) const;

    bool Contains(const ISymbol& symbol) const;
    bool Add(Pointer<ISymbol> symbol);

    const UnresolvedSymbol* GetUnresolvedSymbol() const;
    const UnitTypeSymbol* GetUnitSymbol() const;

    friend class NodeVisitor;

    std::string ToString() const override;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    bool LocalContains(const ISymbol& symbol) const;

    SymbolTable* myParentTable;
    Pointer<UnresolvedSymbol> myUnresolved;
    Pointer<UnitTypeSymbol> myUnitSymbol;
    std::map<std::string, std::vector<Pointer<ISymbol>>> mySymbols;
    //std::set<Pointer<ISymbol>, std::less<>> mySymbols;
};
