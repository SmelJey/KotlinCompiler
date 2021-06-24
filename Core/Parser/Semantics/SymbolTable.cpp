#include "SymbolTable.h"
#include "ClassSymbol.h"
#include "FunctionSymbol.h"

SymbolTable::SymbolTable(const SymbolTable* parent) : myParentTable(parent) {
    myUnresolved = std::make_unique<UnresolvedSymbol>();
    myUnitSymbol = std::make_unique<UnitTypeSymbol>();

    if (parent == nullptr) {
        Add(std::make_unique<IntegerSymbol>());
        Add(std::make_unique<BooleanSymbol>());
        Add(std::make_unique<DoubleSymbol>());
        Add(std::make_unique<StringSymbol>());

        // TODO: decide smthng upon this
        /*IntegerSymbol& integer = static_cast<IntegerSymbol&>(**mySymbols.find(IntegerSymbol()));
        Add(std::make_unique<ArraySymbol>(integer));
        Add(std::make_unique<RangeSymbol>(integer));*/
    }
}

std::vector<const ISymbol*> SymbolTable::GetSymbols(const std::string& name) const {
    std::vector<const ISymbol*> res;
    if (myParentTable != nullptr) {
        res = myParentTable->GetSymbols(name);
    }

    if (mySymbols.count(name)) {
        for (auto& it : mySymbols.at(name)) {
            res.push_back(it.get());
        }
    }

    return res;
}

const ITypeSymbol* SymbolTable::GetType(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    for (auto it : res) {
        auto typeSym = dynamic_cast<const ITypeSymbol*>(it);
        if (typeSym != nullptr) {
            return typeSym;
        }
    }

    return myUnresolved.get();
}

const ITypeSymbol* SymbolTable::GetClass(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    for (auto it : res) {
        auto classSym = dynamic_cast<const ClassSymbol*>(it);
        if (classSym != nullptr) {
            return classSym;
        }
    }

    return myUnresolved.get();
}

std::vector<const FunctionSymbol*> SymbolTable::GetFunctions(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    std::vector<const FunctionSymbol*> funcs;
    for (auto it : res) {
        auto funcSym = dynamic_cast<const FunctionSymbol*>(it);
        if (funcSym != nullptr) {
            funcs.push_back(funcSym);
        }
    }

    return funcs;
}

bool SymbolTable::Contains(const ISymbol& symbol) const {
    std::vector<const ISymbol*> res = GetSymbols(symbol.GetName());
    for (auto it : res) {
        if (*it == symbol) {
            return true;
        }
    }

    return false;
}

bool SymbolTable::Add(Pointer<ISymbol> symbol) {
    if (Contains(*symbol)) {
        return false;
    }

    std::string name = symbol->GetName();
    mySymbols[name].push_back(std::move(symbol));
    return true;
}

const UnresolvedSymbol* SymbolTable::GetUnresolvedSymbol() const {
    return myUnresolved.get();
}

const UnitTypeSymbol* SymbolTable::GetUnitSymbol() const {
    return myUnitSymbol.get();
}
