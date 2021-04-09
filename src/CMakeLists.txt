file(GLOB_RECURSE sources
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.h
        )
add_executable(vcd-to-log ${sources})
add_dependencies(vcd-to-log verilog-vcd-parser xiang-parser)
target_link_libraries(vcd-to-log
        verilog-vcd-parser
        xiang-parser
        )
