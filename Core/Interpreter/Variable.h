#pragma once

#include <any>
#include "../Parser/ParserUtils.h"

class Range;
class Class;
class Array;
class String;
class Boolean;
class Double;
class Integer;

class IVariable {
public:
    virtual ~IVariable() = default;

    template<typename T>
    T GetValue() const {
        return std::any_cast<T>(myValue);
    }

    template<typename T>
    void SetValue(T newVal) {
        myValue.emplace<T>(newVal);
    }

    virtual Pointer<IVariable> Clone() const = 0;

    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const = 0;

    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const Integer* rhs) const;
    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const Double* rhs) const;
    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const Boolean* rhs) const;
    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const String* rhs) const;
    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const Array* rhs) const;
    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const Range* rhs) const;
    virtual Pointer<IVariable> ApplyOperation(LexemeType operation, const Class* rhs) const;

    virtual Pointer<IVariable> ApplyOperation(LexemeType operation) const;

private:
    std::any myValue;
};

class Integer : public IVariable {
public:
    explicit Integer(int value);

    Pointer<IVariable> Clone() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Integer* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Double* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Array* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Range* rhs) const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation) const override;
};

class Double : public IVariable {
public:
    explicit Double(double value);

    Pointer<IVariable> Clone() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Integer* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Double* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Array* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Range* rhs) const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation) const override;
};

class Boolean : public IVariable {
public:
    explicit Boolean(bool value);

    Pointer<IVariable> Clone() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Boolean* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Range* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Array* rhs) const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation) const override;
};

class String : public IVariable {
public:
    explicit String(const std::string& value);

    Pointer<IVariable> Clone() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const String* rhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Array* rhs) const override;
};

class StructArray : public IVariable {
public:
    explicit StructArray(const std::vector<const IVariable*>& src);

    Pointer<IVariable> Clone() const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;

    std::vector<const IVariable*> Get() const;
    const IVariable* Get(int idx) const;
    int Size() const;

    bool In(const IVariable* val) const;

private:
    std::vector<Pointer<IVariable>> myVariables;
};

class Array : public IVariable {
public:
    explicit Array(const StructArray* arr);

    Pointer<IVariable> Clone() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Array* rhs) const override;
};

class StructRange : public IVariable {
public:
    StructRange(const IVariable* left, const IVariable* right);

    Pointer<IVariable> Clone() const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;

    const IVariable* GetLeft() const;
    const IVariable* GetRight() const;

    bool In(const IVariable* val) const;

private:
    Pointer<IVariable> myLeft;
    Pointer<IVariable> myRight;
};

bool operator==(const StructRange& lhs, const StructRange& rhs);
bool operator!=(const StructRange& lhs, const StructRange& rhs);

class Range : public IVariable {
public:
    explicit Range(const StructRange* range);

    Pointer<IVariable> Clone() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Range* rhs) const override;
};

// TODO: make classes
class Class : public IVariable {
public:
    Class();

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const Class* rhs) const override;
};
