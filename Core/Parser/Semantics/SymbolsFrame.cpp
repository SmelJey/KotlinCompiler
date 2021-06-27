#include "SymbolsFrame.h"

SymbolsFrame::SymbolsFrame(SymbolTable** table) : myTablePtr(table) {
    myTable = std::make_unique<SymbolTable>(*table);
    *myTablePtr = myTable.get();
}

SymbolsFrame::~SymbolsFrame() {
    if (!isDisposed) {
        myTable = Dispose();
        (*myTablePtr)->Add(std::move(myTable));
    }
}

Pointer<SymbolTable> SymbolsFrame::Dispose() {
    if (!isDisposed) {
        *myTablePtr = (*myTablePtr)->GetParent();
        isDisposed = true;
        return std::move(myTable);
    }

    return nullptr;
}
