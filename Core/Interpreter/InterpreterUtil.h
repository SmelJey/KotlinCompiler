#pragma once

#include "../Parser/Semantics/SymbolTable.h"
#include "../Parser/Semantics/FunctionSymbol.h"


class InterpreterUtil {
public:
    static const FunctionSymbol* FindMainEntry(const SymbolTable* symbolTable);
};