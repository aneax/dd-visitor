#ifndef DDV_INCLUDE_STATIC_VISITOR_HPP
#define DDV_INCLUDE_STATIC_VISITOR_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <memory>

namespace generic::stat
{

class Bird;
class Sparrow;
class Eagle;

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

    template <typename Visitable, typename Visitor>
    requires std::is_base_of_v<Bird, Visitable>
    static void accept(Visitable& visitable, Visitor& visitor)
    {
      visitor.visit(visitable);
    }

  private:
    std::string name_;
    std::vector<Bird*> children_;
};

class Sparrow : public Bird
{
  public:
    using Bird::Bird;
};

class Eagle : public Bird
{
  public:
    using Bird::Bird;
};

class CountingVisitor
{
  public:
    void visit(Sparrow& sparrow)
    {
      visitors_.push_back(sparrow.name());
      bump();
    }

    void visit(Eagle& eagle)
    {
      visitors_.push_back(eagle.name());
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
}   //namespace generic::stat

namespace sv_cv
{
class Point
{
  public:
    Point() = default;

    Point(double xv, double yv) : x_(xv), y_(yv) {}

    template <typename This, typename Visitor>
    static void accept(This& obj, Visitor& visitor)
    {
      visitor.visit(obj.x_);
      visitor.visit(obj.y_);
    }

    [[nodiscard]] auto x() const noexcept -> double { return x_; }

    [[nodiscard]] auto y() const noexcept -> double { return y_; }

    friend auto operator==(const Point& lhs, const Point& rhs) -> bool
    {
      return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

  private:
    double x_;
    double y_;
};

class Circle
{
  public:
    Circle() = default;

    Circle(Point center, double radius) : center_(center), radius_(radius) {}

    template <typename This, typename Visitor>
    static void accept(This& obj, Visitor& visitor)
    {
      visitor.visit(obj.center_);
      visitor.visit(obj.radius_);
    }

    [[nodiscard]] auto center() const noexcept -> const Point&
    {
      return center_;
    }

    [[nodiscard]] auto radius() const noexcept -> double { return radius_; }

    friend auto operator==(const Circle& lhs, const Circle& rhs) -> bool
    {
      return lhs.center() == rhs.center() && lhs.radius() == rhs.radius();
    }

  private:
    Point center_;
    double radius_;
};

class Line
{
  public:
    Line() = default;

    Line(Point pa, Point pb) : pa_(pa), pb_(pb) {}

    template <typename This, typename Visitor>
    static void accept(This& obj, Visitor& visitor)
    {
      visitor.visit(obj.pa_);
      visitor.visit(obj.pb_);
    }

    [[nodiscard]] auto start() const noexcept -> const Point& { return pa_; }

    [[nodiscard]] auto end() const noexcept -> const Point& { return pb_; }

    friend auto operator==(const Line& lhs, const Line& rhs) -> bool
    {
      return lhs.start() == rhs.start() && lhs.end() == rhs.end();
    }

  private:
    Point pa_;
    Point pb_;
};

template <typename ObjA, typename ObjB>
class Intersection
{
  public:
    Intersection() = default;

    Intersection(ObjA g1, ObjB g2) : g1_(std::move(g1)), g2_(std::move(g2)) {}

    template <typename This, typename Visitor>
    static void accept(This& obj, Visitor& visitor)
    {
      visitor.visit(obj.g1_);
      visitor.visit(obj.g2_);
    }

    [[nodiscard]] auto a() const -> const ObjA& { return g1_; }

    [[nodiscard]] auto b() const -> const ObjB& { return g2_; }

  private:
    ObjA g1_;
    ObjB g2_;
};

class Serializer
{
  public:
    void visit(double value) { ss_ << value << " "; }

    template <typename T>
    void visit(const T& ty)
    {
      T::accept(ty, *this);
    }

    auto string() const -> std::string { return ss_.str(); }

  private:
    std::stringstream ss_;
};

class Deserializer
{
  public:
    Deserializer(const std::string& str) { ss_.str(str); }

    void visit(double& value) { ss_ >> value; }

    template <typename T>
    void visit(const T& ty)
    {
      T::accept(ty, *this);
    }

  private:
    std::stringstream ss_;
};
}   //namespace sv_cv

#endif