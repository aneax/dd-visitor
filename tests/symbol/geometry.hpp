#ifndef DDV_INCLUDE_GEOMETRY_HPP
#define DDV_INCLUDE_GEOMETRY_HPP

#include <cstddef>
#include <memory>
#include <sstream>
#include <utility>

class Point;
class Circle;
class Line;
class Intersection;
class Visitor;

class Geometry
{
  public:
    enum class Type {
      Point,
      Circle,
      Line,
      Intersection
    };

    virtual ~Geometry()                                     = default;
    virtual void accept(Visitor& visitor)                   = 0;
    [[nodiscard]] virtual auto type() const -> Type         = 0;
    [[nodiscard]] virtual auto isa(Type type) const -> bool = 0;
};

class Visitor
{
  public:
    virtual void visit(double& value)           = 0;
    virtual void visit(Point& point)            = 0;
    virtual void visit(Circle& circle)          = 0;
    virtual void visit(Line& line)              = 0;
    virtual void visit(Intersection& intersect) = 0;
    virtual void visit(Geometry::Type& type)    = 0;

    static auto make(Geometry::Type& type) -> std::unique_ptr<Geometry>;
};

class Point : public Geometry
{
  public:
    Point() = default;

    Point(double xv, double yv) : x_(xv), y_(yv) {}

    void accept(Visitor& visitor) override
    {
      visitor.visit(x_);
      visitor.visit(y_);
    }

    [[nodiscard]] auto x() const noexcept -> double { return x_; }

    [[nodiscard]] auto y() const noexcept -> double { return y_; }

    [[nodiscard]] auto type() const -> Type override { return Type::Point; }

    [[nodiscard]] auto isa(Type type) const -> bool override
    {
      return Type::Point == type;
    }

    friend auto operator==(const Point& lhs, const Point& rhs) -> bool
    {
      return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

  private:
    double x_;
    double y_;
};

class Circle : public Geometry
{
  public:
    Circle() = default;

    Circle(Point center, double radius)
        : center_(std::move(center))
        , radius_(radius)
    {
    }

    void accept(Visitor& visitor) override
    {
      visitor.visit(center_);
      visitor.visit(radius_);
    }

    [[nodiscard]] auto center() const noexcept -> const Point&
    {
      return center_;
    }

    [[nodiscard]] auto radius() const noexcept -> double { return radius_; }

    [[nodiscard]] auto type() const -> Type override { return Type::Circle; }

    [[nodiscard]] auto isa(Type type) const -> bool override
    {
      return Type::Circle == type;
    }

    friend auto operator==(const Circle& lhs, const Circle& rhs) -> bool
    {
      return lhs.center() == rhs.center() && lhs.radius() == rhs.radius();
    }

  private:
    Point center_;
    double radius_;
};

class Line : public Geometry
{
  public:
    Line() = default;

    Line(Point pa, Point pb) : pa_(std::move(pa)), pb_(std::move(pb)) {}

    void accept(Visitor& visitor) override
    {
      visitor.visit(pa_);
      visitor.visit(pb_);
    }

    [[nodiscard]] auto start() const noexcept -> const Point& { return pa_; }

    [[nodiscard]] auto end() const noexcept -> const Point& { return pb_; }

    [[nodiscard]] auto type() const -> Type override { return Type::Line; }

    [[nodiscard]] auto isa(Type type) const -> bool override
    {
      return Type::Line == type;
    }

    friend auto operator==(const Line& lhs, const Line& rhs) -> bool
    {
      return lhs.start() == rhs.start() && lhs.end() == rhs.end();
    }

  private:
    Point pa_;
    Point pb_;
};

class Intersection : public Geometry
{
  public:
    Intersection() = default;

    Intersection(std::unique_ptr<Geometry> g1, std::unique_ptr<Geometry> g2)
        : g1_(std::move(g1))
        , g2_(std::move(g2))
    {
    }

    void accept(Visitor& visitor) override
    {
      Geometry::Type type;
      if (g1_) {
        type = g1_->type();
      }
      visitor.visit(type);
      if (!g1_) {
        g1_ = std::move(Visitor::make(type));
      }
      g1_->accept(visitor);

      if (g2_) {
        type = g2_->type();
      }
      visitor.visit(type);
      if (!g2_) {
        g2_ = std::move(Visitor::make(type));
      }
      g2_->accept(visitor);
    }

    [[nodiscard]] auto type() const -> Type override
    {
      return Type::Intersection;
    }

    [[nodiscard]] auto isa(Type type) const -> bool override
    {
      return Type::Intersection == type;
    }

    [[nodiscard]] auto a() const -> const Geometry* { return &(*g1_); }

    [[nodiscard]] auto b() const -> const Geometry* { return &(*g2_); }

  private:
    std::unique_ptr<Geometry> g1_;
    std::unique_ptr<Geometry> g2_;
};

class StringSerializer : public Visitor
{
  public:
    void visit(double& value) override { ss_ << value << " "; }

    void visit(Point& point) override { point.accept(*this); }

    void visit(Circle& circle) override { circle.accept(*this); }

    void visit(Line& line) override { line.accept(*this); }

    void visit(Intersection& intersect) override { intersect.accept(*this); }

    auto string() const -> std::string { return ss_.str(); }

    void visit(Geometry::Type& type) override
    {
      ss_ << static_cast<size_t>(type) << " ";
    }

  private:
    std::stringstream ss_;
};

class StringDeserializer : public Visitor
{
  public:
    StringDeserializer(const std::string& str) { ss_.str(str); }

    void visit(double& value) override { ss_ >> value; }

    void visit(Point& point) override { point.accept(*this); }

    void visit(Circle& circle) override { circle.accept(*this); }

    void visit(Line& line) override { line.accept(*this); }

    void visit(Intersection& intersect) override { intersect.accept(*this); }

    void visit(Geometry::Type& type) override
    {
      size_t size;
      ss_ >> size;
      type = static_cast<Geometry::Type>(size);
    }

  private:
    std::stringstream ss_;
};

auto Visitor::make(Geometry::Type& type) -> std::unique_ptr<Geometry>
{
  switch (type) {
    case Geometry::Type::Point:
      return std::make_unique<Point>();
    case Geometry::Type::Circle:
      return std::make_unique<Circle>();
    case Geometry::Type::Line:
      return std::make_unique<Line>();
    case Geometry::Type::Intersection:
      return std::make_unique<Intersection>();
  }
}

#endif