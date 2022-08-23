#ifndef DDV_INCLUDE_SYMBOL_HPP
#define DDV_INCLUDE_SYMBOL_HPP

#include <string>
#include <vector>

class SymbolVisitor;

enum class Type {
  Int,
  Float,
  String
};

class Symbol
{
    std::string identifier_;

  protected:
    Symbol(std::string_view identifier)
        : identifier_(identifier.begin(), identifier.end())
    {
    }

  public:
    virtual ~Symbol()                                        = default;
    virtual void accept(SymbolVisitor& visitor)              = 0;
    [[nodiscard]] virtual auto type() const noexcept -> Type = 0;

    [[nodiscard]] auto identifier() const -> const std::string&
    {
      return identifier_;
    }
};

#endif