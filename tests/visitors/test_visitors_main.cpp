extern void basic_visitor_test();
extern void generic_visitor_test();
extern void basic_acyclic_visitor_test();
extern void generic_acyclic_visitor_test();
extern void static_visitor_test();
extern void lambda_visitor_test();

int main()   //NOLINT
{
  basic_visitor_test();
  basic_acyclic_visitor_test();
  static_visitor_test();
  generic_visitor_test();
  generic_acyclic_visitor_test();
  lambda_visitor_test();
}