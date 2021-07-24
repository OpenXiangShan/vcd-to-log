add_executable(vcd-to-log
        main.cpp
        Trace.cpp
        Trace.h
        XVisitor.cpp
        XVisitor.h)
add_dependencies(vcd-to-log verilog-vcd-parser xiang-parser vcd-parser-mt)
target_link_libraries(vcd-to-log
        verilog-vcd-parser
        xiang-parser
        vcd-parser-mt
        )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
add_executable(xiang-editor
        ui.cpp
        Trace.cpp
        Trace.h
        XVisitor.h
        XVisitor.cpp)
add_dependencies(xiang-editor verilog-vcd-parser xiang-parser vcd-parser-mt)
target_include_directories(xiang-editor PUBLIC ${GTK3_INCLUDE_DIRS})
target_link_libraries(xiang-editor verilog-vcd-parser xiang-parser vcd-parser-mt ${GTK3_LIBRARIES})



