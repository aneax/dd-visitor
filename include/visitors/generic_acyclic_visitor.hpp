#ifndef DDV_INCLUDE_GENERIC_ACYCLIC_VISITOR_HPP
#define DDV_INCLUDE_GENERIC_ACYCLIC_VISITOR_HPP

#include <cassert>
#include <string>
#include <vector>
#include <memory>

namespace generic::acyclic
{
class BirdMismatch : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class VisitorBase
{
  public:
    virtual ~VisitorBase() = default;
};

template <typename Visitable>
class Visitor
{
  public:
    virtual void visit(Visitable* visitable) = 0;
};

namespace internal
{
template <typename T>
struct SingleVisitors : Visitor<T> {
};

template <typename... Vs>
struct Visitors;

template <typename V>
struct Visitors<V> : public Visitor<V> {
};

template <typename V, typename... Vs>
struct Visitors<V, Vs...> : public Visitor<V>,
                            Visitors<Vs...> {
};
}   //namespace internal

template <typename... Ts>
struct Visitors : internal::SingleVisitors<Ts>... {
};

template <typename Base, typename Visitable>
class VisitableBase : public Base
{
  public:
    using Base::Base;

    void accept(VisitorBase& visitor) override
    {
      if (auto* vs = dynamic_cast<Visitor<Visitable>*>(&visitor)) {
        vs->visit(static_cast<Visitable*>(this));
      } else {
        assert(false);
      }
    }
};

class Bird
{
    std::string name_;

  public:
    Bird(std::string name) : name_(std::move(name)) {}

    [[nodiscard]] auto name() const noexcept -> const std::string&
    {
      return name_;
    }

    virtual ~Bird()                           = default;
    virtual void accept(VisitorBase& visitor) = 0;
};

template <typename Visitable>
using VisitableBird = VisitableBase<Bird, Visitable>;

class Sparrow : public VisitableBird<Sparrow>
{
    using VisitableBird<Sparrow>::VisitableBird;
};

class Eagle : public VisitableBird<Eagle>
{
    using VisitableBird<Eagle>::VisitableBird;
};

class CountingVisitor : public VisitorBase,
                        public Visitors<Eagle, Sparrow>
{
  public:
    void visit([[maybe_unused]] Eagle* eagle) override
    {
      visitors_.push_back(eagle->name());
      ++eagle_;
    }

    void visit([[maybe_unused]] Sparrow* sparrow) override
    {
      visitors_.push_back(sparrow->name());
      ++sparrow_;
    }

    [[nodiscard]] auto sparrow() const -> size_t { return sparrow_; }

    [[nodiscard]] auto eagle() const -> size_t { return eagle_; }

    [[nodiscard]] auto visitors() const -> const std::vector<std::string>&
    {
      return visitors_;
    }

  private:
    size_t sparrow_ = 0;
    size_t eagle_   = 0;
    std::vector<std::string> visitors_;
};

class SmallBirdVisitor : public VisitorBase,
                         public Visitors<Sparrow>
{
  public:
    void visit(Sparrow* sparrow) override { ++sparrow_; }

    [[nodiscard]] auto sparrow() const -> size_t { return sparrow_; }

    [[nodiscard]] auto eagle() const -> size_t { return eagle_; }

  private:
    size_t sparrow_ = 0;
    size_t eagle_   = 0;
};
}   //namespace generic::acyclic
#endif