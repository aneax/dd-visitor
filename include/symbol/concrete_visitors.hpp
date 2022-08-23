#ifndef DDV_INCLUDE_CONCRETE_VISITORS_HPP
#define DDV_INCLUDE_CONCRETE_VISITORS_HPP

#include "concrete_symbols.hpp"
#include "symbol.hpp"
#include "symbol_visitor.hpp"

namespace internal
{
template <typename T>
struct SingleDoNothingVisitor : public virtual SymbolVisitor {
    using SymbolVisitor::visit;

    void visit([[maybe_unused]] T* symbol) override {}
};

template <typename... T>
struct MultipleDoNothingVisitor : public SingleDoNothingVisitor<T>... {
    using SingleDoNothingVisitor<T>::visit...;
};

template <typename T>
struct SingleDefaultVisitor : public virtual SymbolVisitor {
    using SymbolVisitor::visit;
    static_assert(!std::is_same_v<T, Symbol>, "T must not be Symbol");

    void visit([[maybe_unused]] T* symbol) override
    {
      return visit(static_cast<Symbol*>(symbol));
    }
};

template <typename... Ts>
struct MultipleDefaultSymbolVisitor : public SingleDefaultVisitor<Ts>... {
    using SingleDefaultVisitor<Ts>::visit...;
};
}   //namespace internal

struct DoNothingVisitor
    : public internal::MultipleDoNothingVisitor<Symbol, IntSymbol, FloatSymbol,
                                                StringSymbol> {
    using MultipleDoNothingVisitor<Symbol, IntSymbol, FloatSymbol,
                                   StringSymbol>::visit;
};

struct IntFilter : public DoNothingVisitor {
    using DoNothingVisitor::visit;

    void visit(IntSymbol* symbol) override { ints.push_back(symbol); }

    std::vector<IntSymbol*> ints;
};

struct DefaultVisitor
    : public internal::MultipleDefaultSymbolVisitor<IntSymbol, FloatSymbol,
                                                    StringSymbol> {
    using MultipleDefaultSymbolVisitor<IntSymbol, FloatSymbol,
                                       StringSymbol>::visit;
};

struct NotIntFilter : public DefaultVisitor {
    using DefaultVisitor::visit;

    void visit(Symbol* symbol) override { not_ints.push_back(symbol); }

    void visit(IntSymbol* symbol) override {}

    std::vector<Symbol*> not_ints;
};

#endif
