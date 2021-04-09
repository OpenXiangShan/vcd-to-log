#include "Trace.h"

VCDScope* Trace::get_scope_by_path(StrPath& path){
    VecPath vec;
    split_path(path, vec);
    return get_scope_by_path(vec);
}

VCDScope* Trace::get_scope_by_path(VecPath& path){
    VCDScope* scope = trace->get_scope("TOP");
    bool find = false;
    for(auto& s : path){
        for(auto child : scope->children){
            if(s == child->name){
                scope = child;
                find = true;
                break;
            }
        }
        if(!find){
            std::cout << "scope[" << s << "] not found!" << std::endl;
            scope = nullptr;
            break;
        }
        find = false;
    }
    return scope;
}

VCDSignal* Trace::get_signal_by_path(StrPath& path){
    VecPath vec;
    split_path(path, vec);
    if(vec.size() < 2) return nullptr;
    VecPath scope_path(vec.begin(), vec.end() - 1);
    VCDScope* scope = get_scope_by_path(scope_path);
    VCDSignalReference sig = vec.back();
    return get_scope_signal(scope, sig);
}

VCDValue* Trace::get_signal_value_at(
    const VCDSignalHash& hash,
    VCDTime time,
    bool erase_prior
){
    return trace->get_signal_value_at(hash, time, erase_prior);
}

VCDSignalValues* Trace::get_signal_values(VCDSignalHash& hash){
    return trace->get_signal_values(hash);
}

std::vector<VCDTime>* Trace::get_timestamps(){
    return trace->get_timestamps();
}

std::size_t Trace::get_time_size(){
    return trace->get_timestamps()->size();
}

VCDFile* Trace::get_vcd_file(){
    return trace;
}
