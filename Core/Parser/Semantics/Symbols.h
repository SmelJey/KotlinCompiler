#pragma once

#include <memory>
#include <string>
#include "../../Lexer/LexerUtils.h"
#include "../IVisitable.h"

template<typename T>
using Pointer = std::unique_ptr<T>;

class ISymbol : public IVisitable {
public:
    virtual ~ISymbol() = default;
    virtual std::string GetName() const = 0;
    virtual bool operator<(const ISymbol& rhs) const;

    std::string ToString() const override;
};

bool operator!=(const ISymbol& lhs, const ISymbol& rhs);
bool operator==(const ISymbol& lhs, const ISymbol& rhs);

bool operator<(const Pointer<ISymbol>& lhs, const Pointer<ISymbol>& rhs);
bool operator<(const ISymbol& lhs, const Pointer<ISymbol>& rhs);
bool operator<(const Pointer<ISymbol>& lhs, const ISymbol& rhs);

class ITypeSymbol : public ISymbol {
public:
    virtual Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const = 0;
    virtual Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const = 0;
    virtual bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const;
};

class UnresolvedSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class UnitTypeSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class BooleanSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class IntegerSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const override;
};

class DoubleSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
    bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const override;
};

class StringSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;
};

class ArraySymbol : public ITypeSymbol {
public:
    ArraySymbol(const ITypeSymbol* type);

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
    RangeSymbol(const ITypeSymbol& type);

    std::string GetName() const override;
    const ITypeSymbol& GetType() const;

    Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const override;
    Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const override;

private:
    const ITypeSymbol& myType;
};

class VariableSymbol : public ISymbol {
public:
    VariableSymbol(const std::string& name, const ITypeSymbol* type, bool isMutable);

    std::string GetName() const override;
    const ITypeSymbol* GetType() const;

    std::string ToString() const override;

    bool IsMutable() const;

protected:
    void AcceptVisitor(NodeVisitor& visitor, int depth) const override;

private:
    std::string myName;
    const ITypeSymbol* myType;
    bool myMutability;
};
