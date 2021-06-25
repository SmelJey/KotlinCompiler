#include "SymbolTable.h"
#include "ClassSymbol.h"
#include "FunctionSymbol.h"
#include "../NodeVisitor.h"

SymbolTable::SymbolTable(SymbolTable* parent) : myParentTable(parent) {
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

SymbolTable* SymbolTable::GetParent() const {
    return myParentTable;
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

    std::reverse(res.begin(), res.end());
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

const ISymbol* SymbolTable::GetVariable(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    for (auto it : res) {
        auto varSym = dynamic_cast<const VariableSymbol*>(it);
        if (varSym != nullptr) {
            return varSym;
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

const ISymbol* SymbolTable::GetFunction(const std::string& name, const std::vector<const ITypeSymbol*>& params) const {
    std::vector<const FunctionSymbol*> funcs = GetFunctions(name);
    for (auto it : funcs) {
        if (it->GetParametersCount() == params.size()) {
            bool isResolved = true;

            for (int i = 0; i < params.size(); i++) {
                if (*params[i] != it->GetParameter(i)) {
                    isResolved = false;
                    break;
                }
            }

            if (isResolved) {
                return it;
            }
        }
    }

    return myUnresolved.get();
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

const ISymbol* SymbolTable::Add(Pointer<ISymbol> symbol) {
    if (LocalContains(*symbol)) {
        return myUnresolved.get();
    }

    std::string name = symbol->GetName();
    mySymbols[name].push_back(std::move(symbol));
    return mySymbols[name].rbegin()->get();
}

const UnresolvedSymbol* SymbolTable::GetUnresolvedSymbol() const {
    return myUnresolved.get();
}

const UnitTypeSymbol* SymbolTable::GetUnitSymbol() const {
    return myUnitSymbol.get();
}

std::string SymbolTable::ToString() const {
    return "SymbolTable";
}

void SymbolTable::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    for (auto& it : mySymbols) {
        for (auto& sym : it.second) {
            visitor.VisitNode(*sym, depth);
        }
    }
}

bool SymbolTable::LocalContains(const ISymbol& symbol) const {
    if (mySymbols.count(symbol.GetName())) {
        for (auto& it : mySymbols.at(symbol.GetName())) {
            if (*it == symbol) {
                return true;
            }
        }
    }
    return false;
}
