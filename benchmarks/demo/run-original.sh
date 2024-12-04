cd /Users/annyw/Downloads/LLAMA-master/llvm-original/
source build.sh


cd /Users/annyw/Downloads/LLAMA-master/benchmarks/demo

PATH2LIB="/Users/annyw/Downloads/LLAMA-master/llvm-original/build/libllamaPass.so"
PASS=llama-pass 

BENCH=simple_calloc_freq

rm -f default.profraw *_prof *.bc *.profdata 

# Convert source code to bitcode (IR).
clang -emit-llvm -c ${BENCH}.c -Xclang -disable-O0-optnone -o ${BENCH}.bc -Wno-deprecated-non-prototype

# Instrument profiler passes. Generates profile data.
opt -passes='pgo-instr-gen,instrprof' ${BENCH}.bc -o ${BENCH}.prof.bc


#  Generate binary executable with profiler embedded
clang -fprofile-instr-generate -fcoverage-mapping ${BENCH}.prof.bc -o ${BENCH}_prof

# Runs your pass on the instrumented code.
opt  --disable-output -load-pass-plugin="${PATH2LIB}" -passes="${PASS}" ${BENCH}.bc

rm -r /Users/annyw/Downloads/LLAMA-master/llvm-original/build

