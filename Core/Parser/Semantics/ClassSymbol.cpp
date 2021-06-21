#include "ClassSymbol.h"

ClassSymbol::ClassSymbol(const std::string& name, Pointer<SymbolTable> table) : myName(name), myTable(std::move(table)) {}

std::string ClassSymbol::GetName() const {
    return myName;
}
