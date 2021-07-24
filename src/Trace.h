#include <iostream>
#include "VCDFile.hpp"
#include "VCDValue.hpp"
#include "parse_vcd.h"

#ifndef __TRACE_HPP__
#define __TRACE_HPP__

typedef std::string StrPath;
typedef std::vector<std::string> VecPath;

// a wrapper of VCDFile
class Trace {
    private:
        const char* file{};
        VCDFile* trace{};
        std::map<VCDScope *, std::string> name_map;

        static void split_path(StrPath& sp, VecPath& vp, const std::string& delimiters = "."){
            StrPath::size_type lastPos = sp.find_first_not_of(delimiters, 0);
            StrPath::size_type pos = sp.find_first_of(delimiters, lastPos);
            while(StrPath::npos != pos || StrPath::npos != lastPos) {
                vp.push_back(sp.substr(lastPos, pos - lastPos));
                lastPos = sp.find_first_not_of(delimiters, pos);
                pos = sp.find_first_of(delimiters, lastPos);
            }
        }

        void visit_all_scope(VCDScope *cur, std::string &path) {
            for(auto child : cur->children){
                std::string new_path = path.empty() ? child->name : path + "." + child->name;
                name_map[child] = new_path;
                visit_all_scope(child, new_path);
            }
        }

    public:
        explicit Trace(const char* file):file(file)
        {
//            trace = parser.parse_file(file);
            trace = parse_vcd(file);
            if(trace == nullptr){
                std::cerr << "Can't parse file " << file << std::endl;
                exit(-1);
            }
            std::string empty;
            visit_all_scope(trace->root_scope->children[0], empty);
        }

        ~Trace(){
            delete trace;
        }

        std::string get_full_name(VCDScope *scope) {
            return name_map[scope];
        }

        VCDScope* get_scope_by_path(StrPath& path);
        VCDScope* get_scope_by_path(VecPath& path);
        VCDSignal* get_signal_by_path(StrPath& path);

        VCDValue* get_signal_value_at(
            const VCDSignalHash& hash,
            VCDTime time,
            bool erase_prior = false
        );

        VCDSignalValues* get_signal_values(VCDSignalHash& hash);

        std::vector<VCDTime>* get_timestamps();

        std::vector<VCDSignal *>* get_all_signals() {
            return trace->get_signals();
        }

        VCDScope *get_root_scope() {
            return trace->root_scope;
        };

        static VCDSignal* get_scope_signal(VCDScope* scope, VCDSignalReference& name){
            for(auto signal : scope->signals){
                if(signal->reference == name) return signal;
            }
            return nullptr;
        };

        static void print_scope(VCDScope* scope, bool signal = false){
            std::cout << "Scope: " << scope->name << std:: endl;
            std::cout << "Parent: " << scope->parent->name << std::endl;
            std::cout << "Children: ";
            for(auto child : scope->children){
                std::cout << "[" << child->name << "] ";
            }
            std::cout << std::endl;
            if(signal){
                std::cout << "Signals: " << std::endl;
                for(auto sig: scope->signals){
                    print_signal(sig);
                }
            }
        }

        static void print_signal(VCDSignal* sig){
            if(sig->size > 1){
                std::cout << "[" << sig->size << " : 0] ";
            }
            std::cout << sig->reference << std::endl;
        }

    size_t get_time_size();

    VCDFile *get_vcd_file();
};

#endif
