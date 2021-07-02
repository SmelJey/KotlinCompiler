#include "BuiltInTypes.h"

#include "SymbolTable.h"


UnresolvedSymbol::UnresolvedSymbol(SymbolTable* parentTable) : ITypeSymbol("Unresolved type", parentTable) {}

Pointer<ITypeSymbol> UnresolvedSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> UnresolvedSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

UnitTypeSymbol::UnitTypeSymbol(SymbolTable* parentTable) : ITypeSymbol("Unit", parentTable) {}

Pointer<ITypeSymbol> UnitTypeSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> UnitTypeSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*rightOperand == *this) {
        if (LexerUtils::IsEqualityOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>(GetParentTable());
        }
        if (binaryOperation == LexemeType::OpAssign) {
            return std::make_unique<UnitTypeSymbol>(GetParentTable());
        }
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

BooleanSymbol::BooleanSymbol(SymbolTable* parentTable) : ITypeSymbol("Boolean", parentTable) {}

Pointer<ITypeSymbol> BooleanSymbol::IsApplicable(LexemeType operation) const {
    if (operation == LexemeType::OpExclMark) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

// TODO: Refactor
Pointer<ITypeSymbol> BooleanSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*rightOperand == *this) {
        if (LexerUtils::IsBoolOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>(GetParentTable());
        }
        if (binaryOperation == LexemeType::OpDDot) {
            return std::make_unique<RangeSymbol>(GetParentTable(), *this);
        }
        if (binaryOperation == LexemeType::OpAssign) {
            return std::make_unique<BooleanSymbol>(GetParentTable());
        }
    }

    auto range = dynamic_cast<const RangeSymbol*>(rightOperand);
    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn)
        && (range != nullptr && range->GetType() == *this || arr != nullptr)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

IntegerSymbol::IntegerSymbol(SymbolTable* parentTable) : ITypeSymbol("Int", parentTable) {}

Pointer<ITypeSymbol> IntegerSymbol::IsApplicable(LexemeType operation) const {
    if (operation == LexemeType::OpAdd || operation == LexemeType::OpSub || operation == LexemeType::OpInc || operation == LexemeType::OpDec) {
        return std::make_unique<IntegerSymbol>(GetParentTable());
    }
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> IntegerSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        if (LexerUtils::IsBoolOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>(GetParentTable());
        }
        if (LexerUtils::IsArithmeticOperation(binaryOperation)) {
            return std::make_unique<IntegerSymbol>(GetParentTable());
        }
        if (binaryOperation == LexemeType::OpDDot) {
            return std::make_unique<RangeSymbol>(GetParentTable(), *this);
        }
    }

    if (dynamic_cast<const DoubleSymbol*>(rightOperand) != nullptr && LexerUtils::IsArithmeticOperation(binaryOperation)) {
        return std::make_unique<DoubleSymbol>(GetParentTable());
    }

    auto range = dynamic_cast<const RangeSymbol*>(rightOperand);
    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn)
        && (range != nullptr && range->GetType() == *this || arr != nullptr)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

bool IntegerSymbol::IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const {
    bool res = ITypeSymbol::IsAssignable(assignOperation, rightOperand);
    if (!res) {
        res = LexerUtils::IsArithmAssignOperation(assignOperation) && *this == *rightOperand;
    }
    return res;
}

DoubleSymbol::DoubleSymbol(SymbolTable* parentTable) : ITypeSymbol("Double", parentTable) {}

Pointer<ITypeSymbol> DoubleSymbol::IsApplicable(LexemeType operation) const {
    if (operation == LexemeType::OpAdd || operation == LexemeType::OpSub || operation == LexemeType::OpInc || operation == LexemeType::OpDec) {
        return std::make_unique<IntegerSymbol>(GetParentTable());
    }
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> DoubleSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        if (LexerUtils::IsBoolOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>(GetParentTable());
        }
        if (LexerUtils::IsArithmeticOperation(binaryOperation)) {
            return std::make_unique<DoubleSymbol>(GetParentTable());
        }
        if (binaryOperation == LexemeType::OpDDot) {
            return std::make_unique<RangeSymbol>(GetParentTable(), *this);
        }
        if (binaryOperation == LexemeType::OpAssign || LexerUtils::IsArithmAssignOperation(binaryOperation)) {
            return std::make_unique<DoubleSymbol>(GetParentTable());
        }
    }

    if (dynamic_cast<const IntegerSymbol*>(rightOperand) != nullptr && LexerUtils::IsArithmeticOperation(binaryOperation)) {
        return std::make_unique<DoubleSymbol>(GetParentTable());
    }

    auto range = dynamic_cast<const RangeSymbol*>(rightOperand);
    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn)
        && (range != nullptr && range->GetType() == *this || arr != nullptr)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

bool DoubleSymbol::IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        return assignOperation == LexemeType::OpAssign || LexerUtils::IsArithmAssignOperation(assignOperation);
    }

    return false;
}

StringSymbol::StringSymbol(SymbolTable* parentTable) : ITypeSymbol("String", parentTable) {}

Pointer<ITypeSymbol> StringSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> StringSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand) {
        if (LexerUtils::IsEqualityOperation(binaryOperation)) {
            return std::make_unique<BooleanSymbol>(GetParentTable());
        }
        if (binaryOperation == LexemeType::OpAdd) {
            return std::make_unique<StringSymbol>(GetParentTable());
        }
    }

    auto arr = dynamic_cast<const ArraySymbol*>(rightOperand);
    if ((binaryOperation == LexemeType::OpIn || binaryOperation == LexemeType::OpNotIn) && (arr != nullptr)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

ArraySymbol::ArraySymbol(SymbolTable* parentTable, const ITypeSymbol* type) : ITypeSymbol("Array", parentTable), myType(type), mySize(0) {}

std::string ArraySymbol::GetName() const {
    return ITypeSymbol::GetName() + "<" + myType->GetName() + ">";
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
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> ArraySymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

RangeSymbol::RangeSymbol(SymbolTable* parentTable, const ITypeSymbol& type) : ITypeSymbol("ClosedRange", parentTable), myType(type) {}

std::string RangeSymbol::GetName() const {
    return ITypeSymbol::GetName() + "<" + myType.GetName() + ">";
}

const ITypeSymbol& RangeSymbol::GetType() const {
    return myType;
}

Pointer<ITypeSymbol> RangeSymbol::IsApplicable(LexemeType operation) const {
    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}

Pointer<ITypeSymbol> RangeSymbol::IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const {
    if (*this == *rightOperand && LexerUtils::IsEqualityOperation(binaryOperation)) {
        return std::make_unique<BooleanSymbol>(GetParentTable());
    }

    return std::make_unique<UnresolvedSymbol>(GetParentTable());
}
