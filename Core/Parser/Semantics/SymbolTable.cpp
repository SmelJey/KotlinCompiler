#include "SymbolTable.h"

#include "FundamentalType.h"
#include "ClassSymbol.h"
#include "FunctionSymbol.h"
#include "../INodeVisitor.h"

SymbolTable::SymbolTable(SymbolTable* parent) : myParentTable(parent) {
    if (parent == nullptr) {
        myUnresolved = std::make_unique<UnresolvedSymbol>(this);
        myUnitSymbol = std::make_unique<UnitTypeSymbol>(this);

        auto stringSym = dynamic_cast<FundamentalType*>(InnerAdd(std::make_unique<StringSymbol>(this)));
        auto intSym = dynamic_cast<FundamentalType*>(InnerAdd(std::make_unique<IntegerSymbol>(this)));
        auto boolSym = dynamic_cast<FundamentalType*>(InnerAdd(std::make_unique<BooleanSymbol>(this)));
        auto doubleSym = dynamic_cast<FundamentalType*>(InnerAdd(std::make_unique<DoubleSymbol>(this)));

        stringSym->Init();
        intSym->Init();
        boolSym->Init();
        doubleSym->Init();

        std::vector<const AbstractType*> stringParams { stringSym };
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), stringParams, std::make_unique<SymbolTable>(this), nullptr));
        std::vector<const AbstractType*> intParams{ intSym };
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), intParams, std::make_unique<SymbolTable>(this), nullptr));
        std::vector<const AbstractType*> doubleParams{ doubleSym };
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), doubleParams, std::make_unique<SymbolTable>(this), nullptr));
        Add(std::make_unique<FunctionSymbol>("println", myUnitSymbol.get(), std::vector<const AbstractType*>(), std::make_unique<SymbolTable>(this), nullptr));
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

const AbstractType* SymbolTable::GetType(const std::string& name) const {
    std::vector<const ISymbol*> res = GetSymbols(name);
    for (auto it : res) {
        auto typeSym = dynamic_cast<const AbstractType*>(it);
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

const AbstractType* SymbolTable::GetClass(const std::string& name) const {
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

const ISymbol* SymbolTable::GetFunction(const std::string& name, const std::vector<const AbstractType*>& params) const {
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

bool SymbolTable::IsEmpty() const {
    return myBlockTables.empty() && mySymbols.empty();
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
    return InnerAdd(std::move(symbol));
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

void SymbolTable::PropagateVisitor(INodeVisitor& visitor) const {
    for (auto& it : mySymbols) {
        for (auto& sym : it.second) {
            sym->RunVisitor(visitor);
        }
    }
    for (auto& subTable : myBlockTables) {
        subTable->RunVisitor(visitor);
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

ISymbol* SymbolTable::InnerAdd(Pointer<ISymbol> symbol) {
    std::string name = symbol->GetName();
    mySymbols[name].push_back(std::move(symbol));
    return mySymbols[name].rbegin()->get();
}
