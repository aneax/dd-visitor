add_library(Symbol INTERFACE)
target_include_directories(Symbol INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${ALGO_INSTALL_INCLUDE_DIR}>
        )