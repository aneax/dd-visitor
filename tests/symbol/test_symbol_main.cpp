
extern void symbol_visitor_test();
extern void evaluator_test();
extern void symbol_table_test();
extern void serializer_test();

int main()   //NOLINT
{
  symbol_visitor_test();
  evaluator_test();
  symbol_table_test();
  serializer_test();
}