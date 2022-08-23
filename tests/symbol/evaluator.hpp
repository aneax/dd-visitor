#ifndef DDV_INCLUDE_EVALUATOR_HPP
#define DDV_INCLUDE_EVALUATOR_HPP

#include <symbol_visitor.hpp>
#include <concrete_visitors.hpp>
#include <type_traits>

template <typename T, typename... Ts>
struct is_any_one_of : std::disjunction<std::is_same<T, Ts>...> {
};

template <typename T, typename... Ts>
constexpr bool is_any_one_of_v = is_any_one_of<T, Ts...>::value;   //NOLINT

template <typename T>
concept IsNumericSymbol = is_any_one_of_v<T, IntSymbol, FloatSymbol>;

class IntAdderFilter : public DoNothingVisitor
{
  public:
    using DoNothingVisitor::visit;

    void visit(IntSymbol* symbol) override { total_ += symbol->value(); }

    [[nodiscard]] auto value() const noexcept -> int64_t { return total_; }

  private:
    int64_t total_ = 0;
};

class NumericAdder : public DoNothingVisitor
{
  public:
    using DoNothingVisitor::visit;

    void visit(IntSymbol* symbol) override
    {
      total_ += static_cast<double>(symbol->value());
    }

    void visit(FloatSymbol* symbol) override { total_ += symbol->value(); }

    //void visit(NumericSymbol auto* symbol)
    //{
    //  total_ += static_cast<double>(symbol->value());
    //}

    [[nodiscard]] auto value() const noexcept -> double { return total_; }

  private:
    double total_ = 0;
};

#endif