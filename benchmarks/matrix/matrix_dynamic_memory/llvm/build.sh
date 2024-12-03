#!/bin/bash
ml clang
ml cmake/latest

mkdir -p ../build/
cd ../build/
cmake ../llvm
make
