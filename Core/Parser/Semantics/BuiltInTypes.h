#pragma once
#include "Symbols.h"

class FundamentalType : public ITypeSymbol {
public:
    FundamentalType(const std::string& name, SymbolTable* parentTable);

    virtual void Init();

protected:
    void CreateCast(const std::string& castName, const ITypeSymbol* resultType);
};

class UnresolvedSymbol : public FundamentalType {
public:
    explicit UnresolvedSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class UnitTypeSymbol : public FundamentalType {
public:
    explicit UnitTypeSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class BooleanSymbol : public FundamentalType {
public:
    explicit BooleanSymbol(SymbolTable* parentTable);

    void Init() override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class IntegerSymbol : public FundamentalType {
public:
    explicit IntegerSymbol(SymbolTable* parentTable);

    void Init() override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const override;
};

class DoubleSymbol : public FundamentalType {
public:
    explicit DoubleSymbol(SymbolTable* parentTable);

    void Init() override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const override;
};

class StringSymbol : public FundamentalType {
public:
    explicit StringSymbol(SymbolTable* parentTable);

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class ArraySymbol : public FundamentalType {
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

class RangeSymbol : public FundamentalType {
public:
    RangeSymbol(SymbolTable* parentTable, const ITypeSymbol& type);

    std::string GetName() const override;
    const ITypeSymbol& GetType() const;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;

private:
    const ITypeSymbol& myType;
};