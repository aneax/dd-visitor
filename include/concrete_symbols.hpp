#ifndef DDV_INCLUDE_CONCRETE_SYMBOL_HPP
#define DDV_INCLUDE_CONCRETE_SYMBOL_HPP

#include "symbol.hpp"
#include "symbol_visitor.hpp"

#include <cstdint>
#include <string>

template <class T>
class VisitableSymbol : public Symbol
{
  public:
    template <typename... Args>
    VisitableSymbol(Args&&... args) : Symbol(std::forward<Args>(args)...)
    {
    }

    void accept(SymbolVisitor& visitor) override
    {
      visitor.visit(static_cast<T*>(this));
    }
};

class IntSymbol : public VisitableSymbol<IntSymbol>
{
    int64_t value_;

  public:
    //using type = int64_t;

    IntSymbol(std::string_view identifier, int64_t value)
        : VisitableSymbol<IntSymbol>(identifier)
        , value_(value)
    {
    }

    [[nodiscard]] auto value() const noexcept -> int64_t { return value_; }

    [[nodiscard]] auto type() const noexcept -> Type override
    {
      return Type::Int;
    }
};

class FloatSymbol : public VisitableSymbol<FloatSymbol>
{
    double value_;

  public:
    //using type = double;

    FloatSymbol(std::string_view identifier, double value)
        : VisitableSymbol<FloatSymbol>(identifier)
        , value_(value)
    {
    }

    [[nodiscard]] auto value() const -> double { return value_; }

    [[nodiscard]] auto type() const noexcept -> Type override
    {
      return Type::Float;
    }
};

class StringSymbol : public VisitableSymbol<StringSymbol>
{
    std::string value_;

  public:
    //using type = std::string;

    StringSymbol(std::string_view identifier, std::string value)
        : VisitableSymbol<StringSymbol>(identifier)
        , value_(std::move(value))
    {
    }

    [[nodiscard]] auto value() const -> std::string { return value_; }

    [[nodiscard]] auto type() const noexcept -> Type override
    {
      return Type::String;
    }
};

#endif