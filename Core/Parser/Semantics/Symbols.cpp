#include "Symbols.h"

bool ISymbol::operator<(const ISymbol& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return GetName() < rhs.GetName();
    }

    return typeid(*this).before(typeid(rhs));
}

bool operator!=(const ISymbol& lhs, const ISymbol& rhs) {
    return lhs < rhs || rhs < lhs;
}

bool operator==(const ISymbol& lhs, const ISymbol& rhs) {
    return !(lhs != rhs);
}

bool operator<(const Pointer<ISymbol>& lhs, const Pointer<ISymbol>& rhs) {
    return *lhs < *rhs;
    //if (typeid(lhs.get()) == typeid(rhs.get())) {
    //    return *lhs < *rhs;
    //}

    //return typeid(lhs.get()).before(typeid(rhs.get()));
}

bool operator<(const ISymbol& lhs, const Pointer<ISymbol>& rhs) {
    return lhs < *rhs;
    //if (typeid(lhs) == typeid(rhs.get())) {
    //    return lhs < *rhs;
    //}

    //return typeid(lhs).before(typeid(rhs.get()));
}

bool operator<(const Pointer<ISymbol>& lhs, const ISymbol& rhs) {
    return *lhs < rhs;
    //if (typeid(lhs.get()) == typeid(rhs)) {
    //    return *lhs < rhs;
    //}

    //return typeid(lhs.get()).before(typeid(rhs));
}

std::string AutoTypeSymbol::GetName() const {
    return "__TMP__";
}

std::string UnitTypeSymbol::GetName() const {
    return "Unit";
}

std::string UnresolvedTypeSymbol::GetName() const {
    return "Unresolved type";
}

std::string IntegerSymbol::GetName() const {
    return "Int";
}

std::string DoubleSymbol::GetName() const {
    return "Double";
}

std::string StringSymbol::GetName() const {
    return "String";
}

ArraySymbol::ArraySymbol(const ITypeSymbol& type) : myType(type), mySize(0) {}

std::string ArraySymbol::GetName() const {
    return myType.GetName() + "Array";
}

const ITypeSymbol& ArraySymbol::GetType() const {
    return myType;
}

int ArraySymbol::GetSize() const {
    return mySize;
}

void ArraySymbol::SetSize(int size) {
    mySize = size;
}

VariableSymbol::VariableSymbol(const std::string& name, const ITypeSymbol& type) : myName(name), myType(type) {}

std::string VariableSymbol::GetName() const {
    return myName;
}

const ITypeSymbol& VariableSymbol::GetType() const {
    return myType;
}
