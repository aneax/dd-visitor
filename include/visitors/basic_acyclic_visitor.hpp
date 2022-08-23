#ifndef DDV_INCLUDE_BASIC_ACYCLIC_VISITOR_HPP
#define DDV_INCLUDE_BASIC_ACYCLIC_VISITOR_HPP

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

namespace basic::acyclic
{

class BirdMismatch : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class BirdVisitor;

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
    virtual void accept(BirdVisitor& visitor) = 0;
};

class BirdVisitor
{
  public:
    virtual ~BirdVisitor() = default;
};

class Sparrow;
class Eagle;

class SparrowVisitor
{
  public:
    virtual void visit(Sparrow* bird) = 0;
};

class EagleVisitor
{
  public:
    virtual void visit(Eagle* bird) = 0;
};

class Sparrow : public Bird
{
  public:
    Sparrow(const std::string& name) : Bird(name) {}

    void accept(BirdVisitor& visitor) override
    {
      if (auto* mv = dynamic_cast<SparrowVisitor*>(&visitor)) {
        mv->visit(this);
      } else {
        throw BirdMismatch("Must be sparrow");
      }
    }
};

class Eagle : public Bird
{
  public:
    Eagle(const std::string& name) : Bird(name) {}

    void accept(BirdVisitor& visitor) override
    {
      if (auto* mv = dynamic_cast<EagleVisitor*>(&visitor)) {
        mv->visit(this);
      } else {
        throw BirdMismatch("Must be eagle");
      }
    }
};

class CountingVisitor : public BirdVisitor,
                        public SparrowVisitor,
                        public EagleVisitor
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

class SmallBirdVisitor : public BirdVisitor,
                         public SparrowVisitor
{
  public:
    void visit(Sparrow* sparrow) override { ++sparrow_; }

    [[nodiscard]] auto sparrow() const -> size_t { return sparrow_; }

    [[nodiscard]] auto eagle() const -> size_t { return eagle_; }

  private:
    size_t sparrow_ = 0;
    size_t eagle_   = 0;
};

//no need to recompile when another visitable object is added to the hierarchy

//why not to use:
//dynamic_cast (cross_cast) expensive than virtual function call.
//many classes are needed
}   //namespace basic::acyclic

#endif