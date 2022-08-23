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
    using Map = std::unordered_map<std::string, std::shared_ptr<Symbol>>;
    Map table_;

  public:
    template <typename SymbolImpl>
    requires IsSymbol<SymbolImpl>
    void insert(std::shared_ptr<SymbolImpl>&& symbol)
    {
      table_.insert_or_assign(symbol->identifier(), symbol);
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

    auto begin() noexcept -> Map::iterator { return table_.begin(); }

    auto end() noexcept -> Map::iterator { return table_.end(); }

    auto begin() const noexcept -> Map::const_iterator
    {
      return table_.cbegin();
    }

    auto end() const noexcept -> Map::const_iterator { return table_.cend(); }
};

#endif