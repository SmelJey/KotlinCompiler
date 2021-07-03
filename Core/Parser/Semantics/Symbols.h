#pragma once

#include <memory>
#include <string>
#include "../../Lexer/LexerUtils.h"
#include "../IVisitable.h"

class SymbolTable;

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
    ITypeSymbol(const std::string& name, SymbolTable* parentTable);
    ITypeSymbol(const std::string& name, Pointer<SymbolTable> symTable);

    virtual Pointer<ITypeSymbol> IsApplicable(LexemeType operation) const = 0;
    virtual Pointer<ITypeSymbol> IsApplicable(LexemeType binaryOperation, const ITypeSymbol* rightOperand) const = 0;
    virtual bool IsAssignable(LexemeType assignOperation, const ITypeSymbol* rightOperand) const;

    SymbolTable* GetTable() const;

    std::string GetName() const override;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

    SymbolTable* GetParentTable() const;

private:
    SymbolTable* myParentTable;
    std::string myName;
    Pointer<SymbolTable> myTable;
};

class VariableSymbol : public ISymbol {
public:
    VariableSymbol(const std::string& name, const ITypeSymbol* type, bool isMutable);

    std::string GetName() const override;
    const ITypeSymbol* GetType() const;

    std::string ToString() const override;

    bool IsMutable() const;

private:
    std::string myName;
    const ITypeSymbol* myType;
    bool myMutability;
};
