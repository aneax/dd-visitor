#ifndef DDV_INCLUDE_LAMBDA_VISITORS_HPP
#define DDV_INCLUDE_LAMBDA_VISITORS_HPP

#include "generic_visitor.hpp"
#include <utility>

namespace generic::lambda
{

template <typename T>
struct SingleVisitor {
    virtual void visit(T*) = 0;
};

template <typename... T>
struct Visitor : public SingleVisitor<T>... {
    using SingleVisitor<T>::visit...;
};

class Sparrow;
class Eagle;
class Bird;

using BirdVisitor = Visitor<Sparrow, Eagle>;

class Bird
{
  public:
    virtual ~Bird() = default;

    Bird(std::string name) : name_(std::move(name)) {}

    [[nodiscard]] auto name() const -> const std::string& { return name_; }

    void addChild(Bird* bird) { children_.push_back(bird); }

    [[nodiscard]] auto children() const -> std::vector<Bird*>
    {
      return children_;
    }

    virtual void accept(BirdVisitor& visitor) = 0;

  private:
    std::string name_;
    std::vector<Bird*> children_;
};

template <typename BirdImpl>
class Visitable : public Bird
{
  public:
    using Bird::Bird;

    void accept(BirdVisitor& visitor) override
    {
      visitor.visit(static_cast<BirdImpl*>(this));
    }
};

class Sparrow : public Visitable<Sparrow>
{
  public:
    using Visitable<Sparrow>::Visitable;
};

class Eagle : public Visitable<Eagle>
{
  public:
    using Visitable<Eagle>::Visitable;
};

template <typename Base, typename...>
class LambdaVisitor;

template <typename Base, typename T, typename F>
class LambdaVisitor<Base, Visitor<T>, F> : private F,
                                           public Base
{
  public:
    LambdaVisitor(F&& func) : F(std::move(func)) {}

    LambdaVisitor(const F& func) : F{func} {}

    void visit(T* ty) override { return F::operator()(ty); }
};

template <typename Base, typename T1, typename... Ts, typename F1,
          typename... Fs>
class LambdaVisitor<Base, Visitor<T1, Ts...>, F1, Fs...>
    : private F1,
      public LambdaVisitor<Base, Visitor<Ts...>, Fs...>
{
  public:
    LambdaVisitor(F1&& f1, Fs&&... fs)
        : F1(std::move(f1))
        , LambdaVisitor<Base, Visitor<Ts...>, Fs...>(std::forward<Fs>(fs)...)
    {
    }

    LambdaVisitor(const F1& f1, Fs&&... fs)
        : F1(f1)
        , LambdaVisitor<Base, Visitor<Ts...>, Fs...>(std::forward<Fs>(fs)...)
    {
    }

    void visit(T1* t1) override { return F1::operator()(t1); }
};

template <typename Base, typename... Fs>
auto lambda_visitor(Fs&&... fs)
{
  return LambdaVisitor<Base, Base, Fs...>(std::forward<Fs>(fs)...);
}
}   //namespace generic::lambda

#endif