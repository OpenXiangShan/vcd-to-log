#include <iostream>
#include "antlr4-runtime.h"
#include "XiangLexer.h"
#include "XiangParser.h"
#include "XVisitor.h"
#include "getopt.h"
#include "Trace.h"


class ArgParser
{
    private:
        std::string* wave_path = nullptr;
        std::string* xiang_path = nullptr;
        static const struct option long_options[] ;

    public:
        ArgParser(int argc,char** argv);
        std::string* getWavePath();
        std::string* getXiangPath();
        bool SecurityCheck();
        void print_help(const char *file);

};

const struct option ArgParser::long_options[] = {
  { "config",           1, NULL, 'c' },
  { "trace",            1, NULL, 't' },
  { "help",             0, NULL, 'h' },
  { 0,                  0, NULL,  0  }
};

std::string* ArgParser::getWavePath()
{
    return wave_path;
}
std::string* ArgParser::getXiangPath()
{
    return xiang_path;
}
bool ArgParser::SecurityCheck()
{
    if(wave_path && xiang_path) 
        return true;
    else
        return false;
}

void ArgParser::print_help(const char *file) {
  printf("Usage: %s [OPTION...]\n", file);
  printf("\n");
  printf("  -c, --config=FILE        file path to *.xiang file\n");
  printf("  -t, --trace=FILE         file path to *.vcd file\n");
  printf("  -h, --help               print program help info\n");
  printf("\n");
}

ArgParser::ArgParser(int argc,char** argv)
{
    int o;
    while ( (o = getopt_long(argc, const_cast<char *const*>(argv), "-c:t:h", this->long_options, NULL)) != -1) {
    switch (o) {
      case 'c': 
        this->xiang_path = new std::string(optarg);
        break;
      case 't': 
        this->wave_path = new std::string(optarg);
        break;
      default:
        print_help(argv[0]);
        exit(0);
    }
  }
}

int main(int argc, char **argv) {
    // parse args 
    ArgParser* args = new ArgParser(argc,argv);
    if(! args->SecurityCheck())
    {
        args->print_help(argv[0]);
        return 0;
    }

    // parse vcd waveform
    std::string trace_file = *(args->getWavePath());
    auto *trace = new Trace(trace_file.c_str());
    std::cout << "Parse waveform success!" << std::endl;


    // parse *.xiang file
    std::string config_file = *(args->getXiangPath());
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
//"/home/lx/work/vcd/vcd-to-log/examples/TESTFILE/dump.xiang"
//"/home/lx/work/vcd/vcd-to-log/examples/TESTFILE/dump.vcd"
