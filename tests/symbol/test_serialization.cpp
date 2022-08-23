#include "geometry.hpp"
#include <iostream>
#include <boost/ut.hpp>
#include <memory>

//separate implementation of the algorithm from the object that is the data for

//the algorithm allows us to select the correct implementation based on two
//runtime factors- specific object type and concrete operation

//need to maintain two parallel class hierarchies: visitors and visitables


// cycle : visitor object depends on the types of the visitable objects
// the visitable type depends on the base visitor type
extern auto serializer_test()
{
  using namespace boost::ut;
  StringSerializer serializer;
  {
    Line line(Point(0, 0), Point(1, 2));
    Circle circle(Point(0, 0), 5);   //NOLINT
    Intersection intersect(std::make_unique<Circle>(Point(0, 0), 3),
                           std::make_unique<Circle>(Point(2, 1), 2));
    serializer.visit(line);
    serializer.visit(circle);
    serializer.visit(intersect);
  }
  std::string ss(serializer.string());
  //std::cout << ss << "\n";
  expect(ss == "0 0 1 2 0 0 5 1 0 0 3 1 2 1 2 ");

  StringDeserializer deserializer(ss);
  {
    Line line;
    Circle circle;
    Intersection intersect;
    deserializer.visit(line);
    deserializer.visit(circle);
    deserializer.visit(intersect);
    expect(line.start() == Point(0, 0));
    expect(line.end() == Point(1, 2));
    expect(circle.center() == Point(0, 0));
    expect(circle.radius() == 5);   //NOLINT
    expect(intersect.a()->isa(Geometry::Type::Circle));
    expect(intersect.b()->isa(Geometry::Type::Circle));
  }
}