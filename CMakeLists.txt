project(vcd-to-log)

cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_CXX_STANDARD 11)

# verilog-vcd-parser
add_subdirectory(verilog-vcd-parser)
# xiang-parser
add_subdirectory(xiang-parser)
# vcd-to-log
add_subdirectory(src)
