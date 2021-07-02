#include "SymbolTable.h"

#include "BuiltInTypes.h"
#include "ClassSymbol.h"
#include "FunctionSymbol.h"
#include "../INodeVisitor.h"

SymbolTable::SymbolTable(SymbolTable* parent) : myParentTable(parent) {
    if (parent == nullptr) {
        myUnresolved = std::make_unique<UnresolvedSymbol>(this);
        myUnitSymbol = std::make_unique<UnitTypeSymbol>(this);

        auto intSym = dynamic_cast<const ITypeSymbol*>(Add(std::make_unique<IntegerSymbol>(this)));
        Add(std::make_unique<BooleanSymbol>(this));
        auto doubleSym = dynamic_cast<const ITypeSymbol*>(Add(std::make_unique<DoubleSymbol>(this)));
        auto stringSym = dynamic_cast<const ITypeSymbol*>(Add(std::make_unique<StringSymbol>(this)));

        std::vector stringParams { stringSym };
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), stringParams, std::make_unique<SymbolTable>(this)));
        std::vector intParams{ intSym };
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), intParams, std::make_unique<SymbolTable>(this)));
        std::vector doubleParams{ doubleSym };
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), doubleParams, std::make_unique<SymbolTable>(this)));
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), std::vector<const ITypeSymbol*>(), std::make_unique<SymbolTable>(this)));
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

    return GetUnresolvedSymbol();
}

const ISymbol* SymbolTable::GetVariable(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    for (auto it : res) {
        auto varSym = dynamic_cast<const VariableSymbol*>(it);
        if (varSym != nullptr) {
            return varSym;
        }
    }

    return GetUnresolvedSymbol();
}

const ITypeSymbol* SymbolTable::GetClass(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    for (auto it : res) {
        auto classSym = dynamic_cast<const ClassSymbol*>(it);
        if (classSym != nullptr) {
            return classSym;
        }
    }

    return GetUnresolvedSymbol();
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

    return GetUnresolvedSymbol();
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
        return GetUnresolvedSymbol();
    }

    std::string name = symbol->GetName();
    mySymbols[name].push_back(std::move(symbol));
    return mySymbols[name].rbegin()->get();
}

void SymbolTable::Add(Pointer<SymbolTable> table) {
    myBlockTables.push_back(std::move(table));
}

const UnresolvedSymbol* SymbolTable::GetUnresolvedSymbol() const {
    if (myParentTable != nullptr) {
        return myParentTable->GetUnresolvedSymbol();
    }
    return myUnresolved.get();
}

const UnitTypeSymbol* SymbolTable::GetUnitSymbol() const {
    if (myParentTable != nullptr) {
        return myParentTable->GetUnitSymbol();
    }
    return myUnitSymbol.get();
}

std::string SymbolTable::ToString() const {
    return "SymbolTable";
}

void SymbolTable::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    for (auto& it : mySymbols) {
        for (auto& sym : it.second) {
            visitor.VisitNode(*sym, depth);
        }
    }
    for (auto& subTable : myBlockTables) {
        visitor.VisitNode(*subTable, depth);
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
