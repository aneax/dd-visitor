#include <basic_acyclic_visitor.hpp>
#include <generic_acyclic_visitor.hpp>
#include <boost/ut.hpp>

extern void basic_acyclic_visitor_test()
{
  using namespace basic::acyclic;
  using namespace boost::ut;
  auto eagle             = std::make_unique<Eagle>("eagle");
  auto sparrow           = std::make_unique<Sparrow>("sparrow");
  "acyclic_visitor"_test = [&] {
    "counting_visitor"_test = [&] {
      CountingVisitor fv;
      eagle->accept(fv);
      sparrow->accept(fv);
      expect(fv.eagle() == 1);
      expect(fv.sparrow() == 1);
      expect(fv.visitors() == std::vector<std::string>{"eagle", "sparrow"});
    };

    "smallbird_visitor"_test = [&] {
      SmallBirdVisitor bv;
      expect(throws<BirdMismatch>([&] { eagle->accept(bv); }));
      sparrow->accept(bv);
      expect(bv.eagle() == 0);
      expect(bv.sparrow() == 1);
    };
  };
}

extern void generic_acyclic_visitor_test()
{
  using namespace generic::acyclic;
  using namespace boost::ut;
  auto eagle             = std::make_unique<Eagle>("eagle");
  auto sparrow           = std::make_unique<Sparrow>("sparrow");
  "acyclic_visitor"_test = [&] {
    "counting_visitor"_test = [&] {
      CountingVisitor fv;
      eagle->accept(fv);
      sparrow->accept(fv);
      expect(fv.eagle() == 1);
      expect(fv.sparrow() == 1);
      expect(fv.visitors() == std::vector<std::string>{"eagle", "sparrow"});
    };

    "smallbird_visitor"_test = [&] {
      SmallBirdVisitor bv;
      expect(aborts([&] { eagle->accept(bv); }));
      sparrow->accept(bv);
      expect(bv.eagle() == 0);
      expect(bv.sparrow() == 1);
    };
  };
}