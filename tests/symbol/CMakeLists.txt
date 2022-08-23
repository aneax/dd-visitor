add_executable(symbol_tests test_symbol_main.cpp)

target_link_libraries(symbol_tests PRIVATE Boost::ut Symbol)

target_sources(symbol_tests PRIVATE
        test_symbol_visitor.cpp
        test_evaluator.cpp
        test_symbol_table.cpp
        test_serialization.cpp
)

add_test(NAME symbol_tests COMMAND symbol_tests)