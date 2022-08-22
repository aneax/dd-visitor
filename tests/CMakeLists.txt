include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG master
)
FetchContent_MakeAvailable(ut)

add_executable(tests test_main.cpp)
target_link_libraries(tests PRIVATE Boost::ut Symbol)

target_sources(tests PRIVATE
        test_visitor.cpp
        test_evaluator.cpp
)

add_test(NAME tests COMMAND tests)