project(verilog-vcd-parser)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

bison_target(VCDParser VCDParser.ypp ${CMAKE_CURRENT_BINARY_DIR}/VCDParser.cpp
        COMPILE_FLAGS "--defines=VCDParser.hpp"
        )
flex_target(VCDScanner VCDScanner.l ${CMAKE_CURRENT_BINARY_DIR}/VCDScanner.cpp
        COMPILE_FLAGS "-P VCDParser"
        )
add_flex_bison_dependency(VCDScanner VCDParser)

file(GLOB_RECURSE sources
        *.cpp
        *.h
        )

add_library(${PROJECT_NAME}
        ${sources}
        ${BISON_VCDParser_OUTPUTS}
        ${FLEX_VCDScanner_OUTPUTS}
        )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
