#include "InterpreterUtil.h"
#include "../Parser/Semantics/Symbols.h"

const FunctionSymbol* InterpreterUtil::FindMainEntry(const SymbolTable* symbolTable) {
    return dynamic_cast<const FunctionSymbol*>(symbolTable->GetFunction("main", std::vector<const AbstractType*>()));
}

const IVariable* InterpreterUtil::TryDereference(const IVariable* var) {
    auto ref = dynamic_cast<const Reference*>(var);
    if (ref != nullptr) {
        return ref->GetValue<IVariable*>();
    }

    return var;
}

