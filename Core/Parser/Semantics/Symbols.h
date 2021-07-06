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

class AbstractType : public ISymbol {
public:
    AbstractType(const std::string& name, SymbolTable* parentTable);
    AbstractType(const std::string& name, Pointer<SymbolTable> symTable);

    virtual Pointer<AbstractType> IsApplicable(LexemeType operation) const = 0;
    virtual Pointer<AbstractType> IsApplicable(LexemeType binaryOperation, const AbstractType* rightOperand) const = 0;
    virtual bool IsAssignable(LexemeType assignOperation, const AbstractType* rightOperand) const;

    SymbolTable* GetTable() const;

    std::string GetName() const override;

protected:
    void PropagateVisitor(INodeVisitor& visitor) const override;

    SymbolTable* GetParentTable() const;

private:
    SymbolTable* myParentTable;
    std::string myName;
    Pointer<SymbolTable> myTable;
};

class VariableSymbol : public ISymbol {
public:
    VariableSymbol(const std::string& name, const AbstractType* type, bool isMutable);

    std::string GetName() const override;
    const AbstractType* GetType() const;

    std::string ToString() const override;

    bool IsMutable() const;

private:
    std::string myName;
    const AbstractType* myType;
    bool myMutability;
};
