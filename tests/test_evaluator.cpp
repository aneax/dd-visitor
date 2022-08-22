#include <boost/ut.hpp>
#include <concrete_symbols.hpp>
#include "evaluator.hpp"
#include <vector>
#include <string>
#include <memory>

extern void evaluator_test()
{
  //NOLINTBEGIN(readability-*)
  constexpr int64_t a = 25;
  constexpr int64_t d = 35;
  constexpr double b  = 25.5;
  constexpr double e  = 35.5;
  //NOLINTEND(readability-*)
  using namespace boost::ut;

  std::vector<std::unique_ptr<Symbol>> symbols;
  symbols.push_back(std::make_unique<IntSymbol>("a", a));
  symbols.push_back(std::make_unique<FloatSymbol>("b", b));
  symbols.push_back(std::make_unique<StringSymbol>("c", "hello"));
  symbols.push_back(std::make_unique<IntSymbol>("d", d));
  symbols.push_back(std::make_unique<FloatSymbol>("e", e));
  symbols.push_back(std::make_unique<StringSymbol>("f", "world!"));

  "int_adder_filter"_test = [&] {
    IntAdderFilter int_filter;

    for (auto& symbol : symbols) {
      symbol->accept(int_filter);
    }

    expect(int_filter.value() == (a + d));
  };

  "numeric_adder_filter"_test = [&] {
    NumericAdder adder;

    for (auto& symbol : symbols) {
      symbol->accept(adder);
    }

    auto required = a + b + d + e;
    expect(adder.value() == (required));
  };
}