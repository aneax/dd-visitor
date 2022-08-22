#ifndef DDV_INCLUDE_SYMBOL_TABLE_HPP
#define DDV_INCLUDE_SYMBOL_TABLE_HPP

#include "symbol.hpp"
#include <type_traits>
#include <unordered_map>
#include <memory>

template <typename SymbolImpl>
concept IsSymbol = std::is_base_of<Symbol, SymbolImpl>::value
                && requires(SymbolImpl symbol) { symbol.identifier(); };

class SymbolTable
{
  private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> table_;

  public:
    template <typename SymbolImpl>
    requires IsSymbol<SymbolImpl>
    void insert(SymbolImpl&& symbol)
    {
      table_.insert({symbol.identifier(), symbol});
    }

    auto lookup(const std::string& identifier) const noexcept
      -> std::shared_ptr<Symbol>
    {
      auto it = table_.find(identifier);
      if (it == table_.end()) {
        return nullptr;
      }

      return it->second;
    }
};

#endif