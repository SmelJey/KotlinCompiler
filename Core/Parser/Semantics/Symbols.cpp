#include "Symbols.h"

#include "../NodeVisitor.h"

bool ISymbol::operator<(const ISymbol& rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        return GetName() < rhs.GetName();
    }

    return typeid(*this).before(typeid(rhs));
}

std::string ISymbol::ToString() const {
    return GetName();
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

std::string UnitTypeSymbol::GetName() const {
    return "Unit";
}

Pointer<ITypeSymbol> UnitTypeSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> UnitTypeSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*rightOperand == *this) {
        if (LexerUtils::IsEqualityOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>();
        }
        if (binaryOperation == LexemeType::OpAssign) {
            return std::make_unique<UnitTypeSymbol>(*this);
        }
    }

    return std::make_unique<UnresolvedSymbol>();
}

bool ITypeSymbol::IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const {
    return assignOperation == LexemeType::OpAssign && *this == *rightOperand;
}

std::string UnresolvedSymbol::GetName() const {
    return "Unresolved type";
}

Pointer<ITypeSymbol> UnresolvedSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> UnresolvedSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    return std::make_unique<UnresolvedSymbol>();
}

std::string BooleanSymbol::GetName() const {
    return "Boolean";
}

Pointer<ITypeSymbol> BooleanSymbol::IsApplicable(LexemeType operation) const {
    if (operation == LexemeType::OpExclMark) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

// TODO: Refactor
Pointer<ITypeSymbol> BooleanSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*rightOperand == *this) {
        if (LexerUtils::IsBoolOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>();
        }
        if (binaryOperation == LexemeType::OpDDot) {
            return std::make_unique<RangeSymbol>(*this);
        }
        if (binaryOperation == LexemeType::OpAssign) {
            return std::make_unique<BooleanSymbol>();
        }
    }

    auto range = dynamic_cast<const RangeSymbol*>(rightOperand);
    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn)
        && (range != nullptr && range->GetType() == *this || arr != nullptr)) {
        return std::make_unique<BooleanSymbol>();
    }
    
    return std::make_unique<UnresolvedSymbol>();
}

std::string IntegerSymbol::GetName() const {
    return "Int";
}

Pointer<ITypeSymbol> IntegerSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> IntegerSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        if (LexerUtils::IsBoolOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>();
        }
        if (LexerUtils::IsArithmeticOperation(binaryOperation)) {
            return std::make_unique<IntegerSymbol>();
        }
        if (binaryOperation == LexemeType::OpDDot) {
            return std::make_unique<RangeSymbol>(*this);
        }
    }

    if (dynamic_cast<const DoubleSymbol*>(rightOperand) != nullptr && LexerUtils::IsArithmeticOperation(binaryOperation)) {
        return std::make_unique<DoubleSymbol>();
    }

    auto range = dynamic_cast<const RangeSymbol*>(rightOperand);
    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn) 
        && (range != nullptr && range->GetType() == *this || arr != nullptr)) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

bool IntegerSymbol::IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const {
    bool res = ITypeSymbol::IsAssignable(assignOperation, rightOperand);
    if (!res) {
        res = LexerUtils::IsArithmAssignOperation(assignOperation) && *this == *rightOperand;
    }
    return res;
}

std::string DoubleSymbol::GetName() const {
    return "Double";
}

Pointer<ITypeSymbol> DoubleSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> DoubleSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        if (LexerUtils::IsBoolOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>();
        }
        if (LexerUtils::IsArithmeticOperation(binaryOperation)) {
            return std::make_unique<DoubleSymbol>();
        }
        if (binaryOperation == LexemeType::OpDDot) {
            return std::make_unique<RangeSymbol>(*this);
        }
        if (binaryOperation == LexemeType::OpAssign || LexerUtils::IsArithmAssignOperation(binaryOperation)) {
            return std::make_unique<DoubleSymbol>();
        }
    }

    if (dynamic_cast<const IntegerSymbol*>(rightOperand) != nullptr && LexerUtils::IsArithmeticOperation(binaryOperation)) {
        return std::make_unique<DoubleSymbol>();
    }

    auto range = dynamic_cast<const RangeSymbol*>(rightOperand);
    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn)
        && (range != nullptr && range->GetType() == *this || arr != nullptr)) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

bool DoubleSymbol::IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand || dynamic_cast<const IntegerSymbol*>(rightOperand) != nullptr) {
        return assignOperation == LexemeType::OpAssign || LexerUtils::IsArithmAssignOperation(assignOperation);
    }

    return false;
}

std::string StringSymbol::GetName() const {
    return "String";
}

Pointer<ITypeSymbol> StringSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> StringSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        if (LexerUtils::IsEqualityOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>();
        }
        if (binaryOperation == LexemeType::OpAdd) {
            return std::make_unique<StringSymbol>();
        }
    }

    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn) && (arr != nullptr)) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

ArraySymbol::ArraySymbol(const ITypeSymbol* type) : myType(type), mySize(0) {}

std::string ArraySymbol::GetName() const {
    return "Array<" + myType->GetName() + ">";
}

const ITypeSymbol* ArraySymbol::GetType() const {
    return myType;
}

int ArraySymbol::GetSize() const {
    return mySize;
}

void ArraySymbol::SetSize(int size) {
    mySize = size;
}

Pointer<ITypeSymbol> ArraySymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> ArraySymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

RangeSymbol::RangeSymbol(const ITypeSymbol& type) : myType(type) {}

std::string RangeSymbol::GetName() const {
    return "ClosedRange<" + myType.GetName() + ">";
}

const ITypeSymbol& RangeSymbol::GetType() const {
    return myType;
}

Pointer<ITypeSymbol> RangeSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>();
}

Pointer<ITypeSymbol> RangeSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>();
    }

    return std::make_unique<UnresolvedSymbol>();
}

VariableSymbol::VariableSymbol(const std::string& name, const ITypeSymbol* type, bool isMutable)
    : myName(name), myType(type), myMutability(isMutable) {}

std::string VariableSymbol::GetName() const {
    return myName;
}

const ITypeSymbol* VariableSymbol::GetType() const {
    return myType;
}

std::string VariableSymbol::ToString() const {
    return (myMutability ? "Var " : "Val ") + GetName();
}

bool VariableSymbol::IsMutable() const {
    return myMutability;
}

void VariableSymbol::AcceptVisitor(NodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myType, depth);
}

