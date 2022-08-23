add_executable(visitor_tests test_visitors_main.cpp)
target_link_libraries(visitor_tests PRIVATE Boost::ut Visitors)

target_sources(visitor_tests PRIVATE
        test_visitor.cpp
        test_acyclic_visitor.cpp
)

add_test(NAME visitor_tests COMMAND visitor_tests)