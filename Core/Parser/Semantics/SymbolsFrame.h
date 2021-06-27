#pragma once
#include "SymbolTable.h"

class SymbolsFrame {
public:
    explicit SymbolsFrame(SymbolTable** table);

    ~SymbolsFrame();

    Pointer<SymbolTable> Dispose();
private:
    Pointer<SymbolTable> myTable;
    SymbolTable** myTablePtr;
    bool isDisposed = false;
};