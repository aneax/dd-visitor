#include <boost/ut.hpp>
#include <visitor.hpp>
#include <concrete_symbols.hpp>
#include <concrete_visitors.hpp>
#include <vector>
#include <string>
#include <memory>

extern void symbol_visitor_test()
{
  using namespace boost::ut;
  //NOLINTBEGIN(readability-magic-numbers)
  std::vector<std::unique_ptr<Symbol>> symbols;
  symbols.push_back(std::make_unique<IntSymbol>("a", 25));
  symbols.push_back(std::make_unique<FloatSymbol>("b", 25.5));
  symbols.push_back(std::make_unique<StringSymbol>("c", "hello"));
  symbols.push_back(std::make_unique<IntSymbol>("d", 35));
  symbols.push_back(std::make_unique<FloatSymbol>("e", 35.5));
  symbols.push_back(std::make_unique<StringSymbol>("f", "world!"));
  //NOLINTEND(readability-magic-numbers)

  "int_filter"_test = [&] {
    IntFilter int_filter;

    for (auto& symbol : symbols) {
      symbol->accept(int_filter);
    }

    expect(int_filter.ints.size() == 2_i);
    should("contain") = [&int_filter] {
      expect(int_filter.ints[0]->identifier() == "a");
      expect(int_filter.ints[1]->identifier() == "d");
    };
  };

  "not_int_filter"_test = [&] {
    NotIntFilter not_int_filter;
    for (auto& symbol : symbols) {
      symbol->accept(not_int_filter);
    }

    expect(not_int_filter.not_ints.size() == 4_i);
    should("contain") = [&not_int_filter] {
      expect(not_int_filter.not_ints[0]->identifier() == "b");
      expect(not_int_filter.not_ints[1]->identifier() == "c");
      expect(not_int_filter.not_ints[2]->identifier() == "e");
      expect(not_int_filter.not_ints[3]->identifier() == "f");
    };
  };
}