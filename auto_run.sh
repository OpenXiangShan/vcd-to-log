#!/bin/bash

config_path=/home/lx/work/vcd/vcd-to-log/examples/TESTFILE/dump.xiang
trace_path=/home/lx/work/vcd/vcd-to-log/examples/TESTFILE/dump.vcd

cd src && ./vcd-to-log --config=$config_path --trace=$trace_path > result
echo "result in src/result"