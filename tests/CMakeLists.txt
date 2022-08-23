include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG master
)
FetchContent_MakeAvailable(ut)

add_subdirectory(visitors)
add_subdirectory(symbol)



