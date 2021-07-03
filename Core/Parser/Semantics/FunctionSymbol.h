#pragma once
#include <vector>

#include "Symbols.h"
#include "SymbolTable.h"


class AbstractDeclaration;

class FunctionSymbol : public ISymbol {
public:
    FunctionSymbol(const std::string& name, const ITypeSymbol* returnType, const std::vector<const ITypeSymbol*>& params,
        Pointer<SymbolTable> table, const AbstractDeclaration* decl);

    std::string GetName() const override;

    const ITypeSymbol* GetReturnType() const;

    int GetParametersCount() const;

    const ITypeSymbol& GetParameter(int idx) const;
    bool CheckArgument(const ITypeSymbol& type, int idx);

    const AbstractDeclaration* GetDeclaration() const;

    bool operator<(const ISymbol& rhs) const override;

    std::string ToString() const override;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::string myName;
    const ITypeSymbol* myReturnType;
    std::vector<const ITypeSymbol*> myParameters;
    Pointer<SymbolTable> myTable;
    const AbstractDeclaration* myDeclaration;
};
