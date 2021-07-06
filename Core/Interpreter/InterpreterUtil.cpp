#include "InterpreterUtil.h"
#include "../Parser/Semantics/Symbols.h"

const FunctionSymbol* InterpreterUtil::FindMainEntry(const SymbolTable* symbolTable) {
    return dynamic_cast<const FunctionSymbol*>(symbolTable->GetFunction("main", std::vector<const AbstractType*>()));
}

