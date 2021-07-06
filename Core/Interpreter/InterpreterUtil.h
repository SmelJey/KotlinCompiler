#pragma once

#include "Variable.h"
#include "../Parser/Semantics/SymbolTable.h"
#include "../Parser/Semantics/FunctionSymbol.h"


class InterpreterUtil {
public:
    static const FunctionSymbol* FindMainEntry(const SymbolTable* symbolTable);

    static const IVariable* TryDereference(const IVariable* var);

    static Pointer<Reference> CreateReference(IVariable* var);
};