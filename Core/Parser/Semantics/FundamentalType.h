#pragma once
#include "Symbols.h"

class FundamentalType : public AbstractType {
public:
    FundamentalType(const std::string& name, SymbolTable* parentTable);

    virtual void Init();

protected:
    void CreateCast(const std::string& castName, const AbstractType* resultType);
};

class UnresolvedSymbol : public FundamentalType {
public:
    explicit UnresolvedSymbol(SymbolTable* parentTable);

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;
};

class UnitTypeSymbol : public FundamentalType {
public:
    explicit UnitTypeSymbol(SymbolTable* parentTable);

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;
};

class BooleanSymbol : public FundamentalType {
public:
    explicit BooleanSymbol(SymbolTable* parentTable);

    void Init() override;

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;
};

class IntegerSymbol : public FundamentalType {
public:
    explicit IntegerSymbol(SymbolTable* parentTable);

    void Init() override;

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const AbstractType* rightOperand) const override;
};

class DoubleSymbol : public FundamentalType {
public:
    explicit DoubleSymbol(SymbolTable* parentTable);

    void Init() override;

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const AbstractType* rightOperand) const override;
};

class StringSymbol : public FundamentalType {
public:
    explicit StringSymbol(SymbolTable* parentTable);

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;
};

class ArraySymbol : public FundamentalType {
public:
    ArraySymbol(SymbolTable* parentTable, const AbstractType* type);

    std::string GetName() const override;

    const AbstractType* GetType() const;

    int GetSize() const;
    void SetSize(int size);

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;

private:
    const AbstractType* myType;
    int mySize;
};

class RangeSymbol : public FundamentalType {
public:
    RangeSymbol(SymbolTable* parentTable, const AbstractType& type);

    std::string GetName() const override;
    const AbstractType& GetType() const;

    Pointer<AbstractType> IsApplicable(LexemeType operation) const override;
    Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const override;

private:
    const AbstractType& myType;
};