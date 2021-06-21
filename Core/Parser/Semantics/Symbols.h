#pragma once

#include <string>
#include "../SimpleNodes.h"

class ISymbol {
public:
    virtual ~ISymbol() = default;
    virtual std::string GetName() const = 0;
    virtual bool operator<(const ISymbol& rhs) const;
};

bool operator!=(const ISymbol& lhs, const ISymbol& rhs);
bool operator==(const ISymbol& lhs, const ISymbol& rhs);

class ITypeSymbol : public ISymbol {};

bool operator<(const Pointer<ISymbol>& lhs, const Pointer<ISymbol>& rhs);
bool operator<(const ISymbol& lhs, const Pointer<ISymbol>& rhs);
bool operator<(const Pointer<ISymbol>& lhs, const ISymbol& rhs);

class AutoTypeSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;
};

class UnitTypeSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;
};

class UnresolvedTypeSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;
};

class IntegerSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;
};

class DoubleSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;
};

class StringSymbol : public ITypeSymbol {
public:
    std::string GetName() const override;
};

class ArraySymbol : public ITypeSymbol {
public:
    ArraySymbol(const ITypeSymbol& type);

    std::string GetName() const override;

    const ITypeSymbol& GetType() const;

    int GetSize() const;
    void SetSize(int size);

private:
    const ITypeSymbol& myType;
    int mySize;
};

class VariableSymbol : public ISymbol {
public:
    VariableSymbol(const std::string& name, const ITypeSymbol& type);

    std::string GetName() const override;
    const ITypeSymbol& GetType() const;

private:
    std::string myName;
    const ITypeSymbol& myType;
};
