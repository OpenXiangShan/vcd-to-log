project(vcd-parser-mt)
add_library(${PROJECT_NAME}
        parse_vcd.h
        parse_vcd.cpp)
add_dependencies(${PROJECT_NAME} verilog-vcd-parser)
target_link_libraries(${PROJECT_NAME} verilog-vcd-parser)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})