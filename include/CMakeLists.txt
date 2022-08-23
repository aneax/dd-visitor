add_library(Symbol INTERFACE)
target_include_directories(Symbol INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/symbol>
        $<INSTALL_INTERFACE:${ALGO_INSTALL_INCLUDE_DIR}/symbol>
        )

add_library(Visitors INTERFACE)
target_include_directories(Visitors INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/visitors>
        $<INSTALL_INTERFACE:${ALGO_INSTALL_INCLUDE_DIR}/visitors>
        )