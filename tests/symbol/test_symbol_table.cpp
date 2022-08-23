#include "symbol.hpp"
#include <symbol_table.hpp>
#include <concrete_symbols.hpp>
#include <boost/ut.hpp>

extern void symbol_table_test()
{
  using namespace boost::ut;
  //NOLINTBEGIN(readability-identifier-naming)
  constexpr int64_t a = 25;
  constexpr int64_t d = 35;
  constexpr double b  = 25.5;
  constexpr double e  = 35.5;
  //NOLINTEND(readability-identifier-naming)

  SymbolTable table;
  table.insert(std::make_shared<IntSymbol>("a", a));
  table.insert(std::make_shared<FloatSymbol>("b", b));
  table.insert(std::make_shared<StringSymbol>("c", "hello"));
  table.insert(std::make_shared<IntSymbol>("d", d));
  table.insert(std::make_shared<FloatSymbol>("e", e));
  table.insert(std::make_shared<StringSymbol>("f", "world!"));

  "in_table"_test = [&] {
    {
      auto value = table.lookup("a");
      expect(value->identifier() == "a");
      expect(value->type() == Type::Int);

      auto* in = static_cast<IntSymbol*>(value.get());
      expect(in->value() == a);
    }

    {
      auto value = table.lookup("b");
      expect(value->identifier() == "b");
      expect(value->type() == Type::Float);
      auto* in = static_cast<FloatSymbol*>(value.get());
      expect(in->value() == b);
    }

    {
      auto value = table.lookup("c");
      expect(value->identifier() == "c");
      expect(value->type() == Type::String);
      auto* in = static_cast<StringSymbol*>(value.get());
      expect((in->value() == "hello") >> fatal);
    }
  };

  "not_in_table"_test = [&] {
    {
      auto value = table.lookup("k");
      expect((value == nullptr)) << "Must be nullptr";
    }
    {
      auto value = table.lookup("z");
      expect((value == nullptr) >> fatal);
    }

    class A
    {
    };
    static_assert(!IsSymbol<A>, "A is not a Symbol");
    static_assert(IsSymbol<IntSymbol>, "IntSymbol is a Symbol");
    static_assert(IsSymbol<FloatSymbol>, "FloatSymbol is a Symbol");
    static_assert(IsSymbol<StringSymbol>, "StringSymbol is a Symbol");

    "reinsertion"_test = [&] {
      table.insert(std::make_shared<FloatSymbol>("a", b));
      table.insert(std::make_shared<IntSymbol>("b", a));
      {
        auto value = table.lookup("a");
        expect(value->identifier() == "a");
        expect((value->type() == Type::Float) >> fatal);

        auto* in = static_cast<FloatSymbol*>(value.get());
        expect(in->value() == b);
      }
      {
        auto value = table.lookup("b");
        expect(value->identifier() == "b");
        expect((value->type() == Type::Int) >> fatal);

        auto* in = static_cast<IntSymbol*>(value.get());
        expect(in->value() == a);
      }
    };
  };
}