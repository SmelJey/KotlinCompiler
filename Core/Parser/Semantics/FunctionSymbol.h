#pragma once
#include <vector>

#include "Symbols.h"
#include "SymbolTable.h"


class AbstractDeclaration;

class FunctionSymbol : public ISymbol {
public:
    FunctionSymbol(const std::string& name, const AbstractType* returnType, const std::vector<const AbstractType*>& params,
        Pointer<SymbolTable> table, const AbstractDeclaration* decl);

    std::string GetName() const override;

    const AbstractType* GetReturnType() const;

    int GetParametersCount() const;

    const AbstractType& GetParameter(int idx) const;
    bool CheckArgument(const AbstractType& type, int idx);

    const AbstractDeclaration* GetDeclaration() const;

    bool operator<(const ISymbol& rhs) const override;

    std::string ToString() const override;

protected:
    void AcceptVisitor(INodeVisitor& visitor) const override;

private:
    std::string myName;
    const AbstractType* myReturnType;
    std::vector<const AbstractType*> myParameters;
    Pointer<SymbolTable> myTable;
    const AbstractDeclaration* myDeclaration;
};
