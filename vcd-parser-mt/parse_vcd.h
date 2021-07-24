//
// Created by ljw on 2021/7/24.
//

#ifndef VCD_TO_LOG_PARSE_VCD_H
#define VCD_TO_LOG_PARSE_VCD_H

#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include "VCDFile.hpp"
#include "VCDValue.hpp"

VCDFile *parse_vcd(const char *fpath);

#endif //VCD_TO_LOG_PARSE_VCD_H
