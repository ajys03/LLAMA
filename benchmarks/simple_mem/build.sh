#!/bin/bash

#clang -emit-llvm -S simple_calloc.c -o simple.bc
#clang -emit-llvm -S empty.c -o simple.bc
#opt -S -loop-rotate < simple.bc > simple2.bc
#opt -S -load-pass-plugin=/Users/annyw/Downloads/LLAMA-master/build/libllamaPass.so -passes=llama < simple.bc > simple1.bc
#llc -filetype=obj simple1.bc
#clang simple1.o library/0.a -o simple_0
clang empty.c -o simple_0
# clang simple3.o library/1000.a -o simple_1000
# clang simple3.o library/10000.a -o simple_10000
# clang simple3.o library/20000.a -o simple_20000
# clang simple3.o library/30000.a -o simple_30000
# clang simple3.o library/40000.a -o simple_40000
