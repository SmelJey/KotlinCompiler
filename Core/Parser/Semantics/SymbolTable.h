#pragma once
#include <map>
#include <string>
#include <vector>

#include "Symbols.h"

class UnitTypeSymbol;
class UnresolvedSymbol;
class FunctionSymbol;

class SymbolTable : public IVisitable {
public:
    SymbolTable(SymbolTable* parent = nullptr);
    SymbolTable(SymbolTable& src) = delete;

    SymbolTable* GetParent() const;
    std::vector<const ISymbol*> GetSymbols(const std::string& name) const;

    const AbstractType* GetType(const std::string& name) const;
    const ISymbol* GetVariable(const std::string& name) const;
    const AbstractType* GetClass(const std::string& name) const;
    std::vector<const FunctionSymbol*> GetFunctions(const std::string& name) const;

    const ISymbol* GetFunction(const std::string& name, const std::vector<const AbstractType*>& params) const;

    bool IsEmpty() const;
    bool Contains(const ISymbol& symbol) const;
    const ISymbol* Add(Pointer<ISymbol> symbol);
    void Add(Pointer<SymbolTable> table);

    const UnresolvedSymbol* GetUnresolvedSymbol() const;
    const UnitTypeSymbol* GetUnitSymbol() const;

    friend class INodeVisitor;

    std::string ToString() const override;

protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

private:
    bool LocalContains(const ISymbol& symbol) const;
    ISymbol* InnerAdd(Pointer<ISymbol> symbol);

    SymbolTable* myParentTable;
    Pointer<UnresolvedSymbol> myUnresolved;
    Pointer<UnitTypeSymbol> myUnitSymbol;
    std::map<std::string, std::vector<Pointer<ISymbol>>> mySymbols;
    std::vector<Pointer<SymbolTable>> myBlockTables;
};
