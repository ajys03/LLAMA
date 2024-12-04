#!/bin/bash
# Run script for Homework 1 EECS 583 Winter 2024
# Place this script inside the benchmark directory. e.g. benchmark1/run.sh
# Usage: sh run.sh <benchmark_name>
# where <benchmark_name> = simple OR anagram OR compress

PATH2LIB="/Users/annyw/Downloads/LLAMA-master/build/libllamaPass.so"
PASS=llama-pass 

BENCH=${1}.c

if [ -z "$1" ]; then
    echo "Usage: sh run.sh <benchmark_name>"
    exit 1
fi

# # Delete outputs from previous runs. Update this if you want to retain some files across runs.
# rm -f default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll *.in *.in.Z

# # Convert source code to bitcode (IR).
clang -emit-llvm -c ${BENCH} -Xclang -disable-O0-optnone -o ${1}.bc -Wno-deprecated-non-prototype

# # Instrument profiler passes. Generates profile data.
opt -passes='pgo-instr-gen,instrprof' ${1}.bc -o ${1}.prof.bc

# # # Generate binary executable with profiler embedded
clang -fprofile-instr-generate -fcoverage-mapping ${1}.prof.bc -o ${1}_prof


# ./${1}_prof > /dev/null

# # # Converting it to LLVM form. This step can also be used to combine multiple profraw files,
# # # in case you want to include different profile runs together.
# llvm-profdata merge -o ${1}.profdata default.profraw

# # # The "Profile Guided Optimization Instrumentation-Use" pass attaches the profile data to the bc file.
opt -passes="pgo-instr-use" -o ${1}.profdata.bc -pgo-test-profile-file=${1}.profdata < ${1}.bc

# # Uncomment this and disable the cleanup if you want to "see" the instumented IR.
#llvm-dis ${1}.profdata.bc -o ${1}.prof.ll

#clang -emit-llvm -c ${BENCH} -Xclang -disable-O0-optnone -o ${1}.bc -Wno-deprecated-non-prototype

# Runs your pass on the instrumented code.
opt  --disable-output -load-pass-plugin="${PATH2LIB}" -passes="${PASS}" ${1}.bc
