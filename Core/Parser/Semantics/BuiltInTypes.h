#pragma once
#include "Symbols.h"

class UnresolvedSymbol : public ITypeSymbol {
public:
    explicit UnresolvedSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class UnitTypeSymbol : public ITypeSymbol {
public:
    explicit UnitTypeSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class BooleanSymbol : public ITypeSymbol {
public:
    explicit BooleanSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class IntegerSymbol : public ITypeSymbol {
public:
    explicit IntegerSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const override;
};

class DoubleSymbol : public ITypeSymbol {
public:
    explicit DoubleSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const override;
};

class StringSymbol : public ITypeSymbol {
public:
    explicit StringSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class ArraySymbol : public ITypeSymbol {
public:
    ArraySymbol(SymbolTable* parentTable, const ITypeSymbol* type);

    std::string GetName() const override;

    const ITypeSymbol* GetType() const;

    int GetSize() const;
    void SetSize(int size);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;

private:
    const ITypeSymbol* myType;
    int mySize;
};

class RangeSymbol : public ITypeSymbol {
public:
    RangeSymbol(SymbolTable* parentTable, const ITypeSymbol& type);

    std::string GetName() const override;
    const ITypeSymbol& GetType() const;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;

private:
    const ITypeSymbol& myType;
};