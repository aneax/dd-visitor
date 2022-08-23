#ifndef DDV_INCLUDE_GENERIC_VISITOR_HPP
#define DDV_INCLUDE_GENERIC_VISITOR_HPP

#include <cassert>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>
#include <sstream>

namespace generic::generic
{

template <typename Base, typename T>
struct SingleVisitor {
    virtual void visit(T*, Base* = nullptr) = 0;
};

template <typename Base, typename... T>
struct Visitor : public SingleVisitor<Base, T>... {
    using SingleVisitor<Base, T>::visit...;
};

class Sparrow;
class Eagle;
class Bird;

using BirdVisitor = Visitor<Bird, Sparrow, Eagle>;

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

    virtual void accept(BirdVisitor& visitor, Bird* bird = nullptr) = 0;

  private:
    std::string name_;
    std::vector<Bird*> children_;
};

template <typename BirdImpl>
class Visitable : public Bird
{
  public:
    using Bird::Bird;

    void accept(BirdVisitor& visitor, Bird* bird = nullptr) override
    {
      visitor.visit(static_cast<BirdImpl*>(this), bird);
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

class Sanctuary
{
  public:
    void add(std::unique_ptr<Bird> bird) { birds_.push_back(std::move(bird)); }

    template <typename BirdImpl>
    requires std::is_base_of_v<Bird, BirdImpl>
    void add(BirdImpl&& bird)
    {
      birds_.push_back(std::make_unique<BirdImpl>(bird));
    }

    void accept(BirdVisitor& visitor)
    {
      for (auto& bird : birds_) {
        bird->accept(visitor);
      }
    }

  private:
    std::vector<std::unique_ptr<Bird>> birds_;
};

class CountingVisitor : public BirdVisitor
{
  public:
    void visit(Sparrow* sparrow, Bird* bird = nullptr) override
    {
      visitors_.push_back(sparrow->name());
      bump();
    }

    void visit(Eagle* eagle, Bird* bird = nullptr) override
    {
      visitors_.push_back(eagle->name());
      bump();
    }

    void bump() { ++fed_; }

    [[nodiscard]] auto fed() const -> size_t { return fed_; }

    [[nodiscard]] auto visitors() const -> const std::vector<std::string>&
    {
      return visitors_;
    }

  private:
    size_t fed_ = 0;
    std::vector<std::string> visitors_;
};

class BirthVisitor : public BirdVisitor
{
  public:
    void visit(Sparrow* sparrow, Bird* bird) override
    {
      assert(dynamic_cast<Sparrow*>(bird));
      sparrow->addChild(bird);
    }

    void visit(Eagle* eagle, Bird* bird) override
    {
      assert(dynamic_cast<Eagle*>(bird));
      eagle->addChild(bird);
    }
};

class FamilyTreeVisitor : public BirdVisitor
{
  public:
    void visit(Sparrow* sparrow, Bird* /*bird*/) override
    {
      ss_ << sparrow->name() << ":\n";
      for (auto* kit : sparrow->children()) {
        ss_ << kit->name() << " ";
      }
      ss_ << "\n";
    }

    void visit(Eagle* eagle, Bird* /*bird*/) override
    {
      ss_ << eagle->name() << ":\n";
      for (auto* pup : eagle->children()) {
        ss_ << pup->name() << " ";
      }
      ss_ << "\n";
    }

    auto string() const -> std::string { return ss_.str(); }

  private:
    std::stringstream ss_;
};

}   //namespace generic::generic

#endif