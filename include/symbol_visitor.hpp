#ifndef DDV_INCLUDE_SYMBOL_VISITOR_HPP
#define DDV_INCLUDE_SYMBOL_VISITOR_HPP

#include "visitor.hpp"
#include <vector>

class Symbol;
class IntSymbol;
class FloatSymbol;
class StringSymbol;

struct SymbolVisitor
    : public virtual Visitor<Symbol, IntSymbol, FloatSymbol, StringSymbol> {
    using Visitor<Symbol, IntSymbol, FloatSymbol, StringSymbol>::visit;
};

#endif