#!/bin/bash

# Paths and variables
# CHANGE to your root directory
ROOT_DIR=""
LLVM_DIR="${ROOT_DIR}/llvm-original"
BENCH_DIR="${ROOT_DIR}/benchmarks/demo"
PATH2LIB="${LLVM_DIR}/build/libllamaPass.so"
PASS="llama-pass"

clang -emit-llvm -S matrix.c -o simple.bc
#opt -S -loop-rotate < simple.bc > simple2.bc
opt -S -load-pass-plugin=${PASS2LIB} -passes=${PASS} < simple.bc > simple1.bc
llc -filetype=obj simple1.bc
#clang simple3.o library/llama.a -o matrix_0
clang simple1.o library/0.a -o matrix_0
# clang simple3.o library/10.a -o matrix_10
# clang simple3.o library/100.a -o matrix_100
# clang simple3.o library/1000.a -o matrix_1000
# clang simple3.o library/10000.a -o matrix_10000
# clang simple3.o library/20000.a -o matrix_20000
# clang simple3.o library/30000.a -o matrix_30000
# clang simple3.o library/40000.a -o matrix_40000
# clang simple3.o library/100000.a -o matrix_100000
