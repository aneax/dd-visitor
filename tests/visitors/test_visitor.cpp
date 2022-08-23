
#include <basic_visitor.hpp>
#include <generic_visitor.hpp>
#include <static_visitor.hpp>
#include <lambda_visitor.hpp>
#include <boost/ut.hpp>
#include <memory>

extern void basic_visitor_test()
{
  using namespace basic::basic;
  using namespace boost::ut;
  auto sparrow = std::make_unique<Sparrow>("puspus");
  auto eagle   = std::make_unique<Eagle>("fluffy");

  "basic_visitor"_test = [&] {
    "feeding_visitor"_test = [&] {
      CountingVisitor fv;
      sparrow->accept(fv);
      eagle->accept(fv);

      expect(fv.fed() == 2);
    };

    "birth_visitor"_test = [&] {
      BirthVisitor bv;
      Sparrow cchild("toby");
      Eagle dchild("ruso");

      double_dispatch(&(*sparrow), &bv, &cchild);
      double_dispatch(&(*eagle), &bv, &dchild);

      expect(sparrow->children()[0]->name() == "toby");
      expect(eagle->children()[0]->name() == "ruso");
    };

    "santuary"_test = [&] {
      Sanctuary sancturay;
      sancturay.add(std::make_unique<Sparrow>("sparrow_1"));
      sancturay.add(std::make_unique<Sparrow>("sparrow_2"));
      sancturay.add(std::make_unique<Eagle>("eagle_2"));
      sancturay.add(std::make_unique<Eagle>("eagle_1"));
      sancturay.add(Eagle("eagle_3"));
      sancturay.add(Sparrow("sparrow_3"));

      CountingVisitor cv;
      sancturay.accept(cv);
      expect(cv.fed() == 6);
      expect(cv.visitors()
             == std::vector<std::string>{"sparrow_1", "sparrow_2", "eagle_2",
                                         "eagle_1", "eagle_3", "sparrow_3"});
    };
  };
}

extern void generic_visitor_test()
{
  using namespace generic::generic;
  using namespace boost::ut;
  auto sparrow = std::make_unique<Sparrow>("puspus");
  auto eagle   = std::make_unique<Eagle>("fluffy");

  "generic_visitor"_test = [&] {
    "feeding_visitor"_test = [&] {
      CountingVisitor fv;
      sparrow->accept(fv);
      eagle->accept(fv);

      expect(fv.fed() == 2);
    };

    "birth_visitor"_test = [&] {
      BirthVisitor bv;
      Sparrow cchild("toby");
      Eagle dchild("ruso");

      sparrow->accept(bv, &cchild);
      eagle->accept(bv, &dchild);

      expect(sparrow->children()[0]->name() == "toby");
      expect(eagle->children()[0]->name() == "ruso");

      expect(aborts([&]() { sparrow->accept(bv, &dchild); }));
      expect(aborts([&]() { eagle->accept(bv, &cchild); }));
    };

    "santuary"_test = [&] {
      Sanctuary sancturay;
      sancturay.add(std::make_unique<Sparrow>("sparrow_1"));
      sancturay.add(std::make_unique<Sparrow>("sparrow_2"));
      sancturay.add(std::make_unique<Eagle>("eagle_2"));
      sancturay.add(std::make_unique<Eagle>("eagle_1"));
      sancturay.add(Eagle("eagle_3"));
      sancturay.add(Sparrow("sparrow_3"));

      CountingVisitor cv;
      sancturay.accept(cv);
      expect(cv.fed() == 6);
      expect(cv.visitors()
             == std::vector<std::string>{"sparrow_1", "sparrow_2", "eagle_2",
                                         "eagle_1", "eagle_3", "sparrow_3"});
    };
  };
}

extern void static_visitor_test()
{
  using namespace generic::stat;
  using namespace boost::ut;
  auto sparrow = std::make_unique<Sparrow>("puspus");
  auto eagle   = std::make_unique<Eagle>("fluffy");

  "generic_visitor"_test = [&] {
    "feeding_visitor"_test = [&] {
      CountingVisitor fv;
      Bird::accept(*sparrow, fv);
      Bird::accept(*eagle, fv);

      expect(fv.fed() == 2);
    };
  };
}

extern void lambda_visitor_test()
{
  using namespace generic::lambda;
  using namespace boost::ut;
  auto sparrow = std::make_unique<Sparrow>("sparrow");
  auto eagle   = std::make_unique<Eagle>("eagle");

  "generic_lambda_visitor"_test = [&] {
    using BVisitor = Visitor<Sparrow, Eagle>;

    std::string out;
    auto visitor(lambda_visitor<BVisitor>(
      [&out](Sparrow* sparrow) { out = sparrow->name(); },
      [&out](Eagle* eagle) { out = eagle->name(); }));

    sparrow->accept(visitor);
    expect(out == "sparrow");
    eagle->accept(visitor);
    expect(out == "eagle");
  };
}