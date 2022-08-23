#ifndef DDV_INCLUDE_BASIC_VISITOR_HPP
#define DDV_INCLUDE_BASIC_VISITOR_HPP

#include <sstream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <cassert>

namespace basic::basic
{

class Bird;
class Sparrow;
class Eagle;

class BirdVisitor
{
  public:
    virtual void visit(Sparrow* sparrow, Bird* bird = nullptr) = 0;
    virtual void visit(Eagle* eagle, Bird* bird = nullptr)     = 0;
};

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

class Sparrow : public Bird
{
  public:
    Sparrow(const std::string& name) : Bird(name) {}

    void accept(BirdVisitor& visitor, Bird* bird = nullptr) override
    {
      visitor.visit(this, bird);
    }
};

class Eagle : public Bird
{
  public:
    Eagle(const std::string& name) : Bird(name) {}

    void accept(BirdVisitor& visitor, Bird* bird = nullptr) override
    {
      visitor.visit(this, bird);
    }
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

inline void double_dispatch(Bird* bird, BirdVisitor* visitor,
                            Bird* child = nullptr)
{
  bird->accept(*visitor, child);
}

//inline void walk(Pet& bird)
//{
//  //Pet visitor is not a template
//  auto vr(lambda_visitor<PetVisitor>([](Sparrow* sparrow) {}, [](Eagle* eagle)
//  {})); bird.accept(vr);
//}
}   //namespace basic::basic
#endif