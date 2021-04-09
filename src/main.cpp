#include <iostream>
#include "antlr4-runtime.h"
#include "XiangLexer.h"
#include "XiangParser.h"
#include "XVisitor.h"
#include "Trace.h"

int main(int argc, char **argv) {
    // parse vcd waveform
    std::string trace_file = "/home/lin/Documents/project/vcd-to-log/examples/test.vcd";
    auto *trace = new Trace(trace_file.c_str());
    std::cout << "Parse waveform success!" << std::endl;


    // parse *.xiang file
    std::string config_file = "/home/lin/Documents/project/vcd-to-log/examples/test.xiang";
    std::ifstream ifstream;
    ifstream.open(config_file);
    antlr4::ANTLRInputStream inputStream(ifstream);
    XiangLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokenStream(&lexer);
    XiangParser parser(&tokenStream);
    std::cout << "Parse .xiang file success!" << std::endl;

    // create visitor
    auto *xVisitor = new XVisitor(trace);
    std::cout << (*trace->get_timestamps()).size() << std::endl;
    std::cout << (trace->get_timestamps() == nullptr) << std::endl;
    auto tree = parser.main();
    for(VCDTime t : *trace->get_timestamps()){
        xVisitor->set_time(t);
        xVisitor->visitMain(tree);
    }
    delete(xVisitor);
    return 0;
}
