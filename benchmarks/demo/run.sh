# Paths and variables
# CHANGE to your root directory
ROOT_DIR="/Users/annyw/Downloads/LLAMA-master"
LLVM_DIR="${ROOT_DIR}/llvm"
BENCH_DIR="${ROOT_DIR}/benchmarks/demo"
PATH2LIB="${LLVM_DIR}/build/libllamaPass.so"
PASS="llama-pass"

#Build the llama-pass
cd ${LLVM_DIR}
source build.sh

cd ${BENCH_DIR}
BENCH=simple_calloc_freq

# Delete outputs from previous runs
rm -f default.profraw *_prof *.bc *.profdata *_output

# Convert source code to bitcode (IR).
clang -mmacosx-version-min=14.7 -emit-llvm -c ${BENCH}.c -Xclang -disable-O0-optnone -o ${BENCH}.bc -Wno-deprecated-non-prototype

# Rotate loops
#opt -passes="loop-rotate" ${BENCH}.bc -o ${BENCH}.lr.bc

# Instrument profiler passes. Generates profile data.
opt -passes='pgo-instr-gen,instrprof' ${BENCH}.bc -o ${BENCH}.prof.bc

#  Generate binary executable with profiler embedded
clang -mmacosx-version-min=14.7 -fprofile-instr-generate -fcoverage-mapping ${BENCH}.prof.bc -o ${BENCH}_prof

# Run the binary to generate profile data
./${BENCH}_prof > correct_output.txt

# Check if profile data is generated
if [ ! -f "default.profraw" ]; then
  echo "Error: Profile data default.profraw not generated."
  exit 1
fi

# Merge profile data
llvm-profdata merge -o ${BENCH}.profdata default.profraw

# Attach profile data to the bitcode
# CHANGE the input file to ${BENCH}.lr.bc if applying loop-rotate pass
opt -passes="pgo-instr-use" -o ${BENCH}.profdata.bc -pgo-test-profile-file=${BENCH}.profdata < ${BENCH}.bc > /dev/null

# Run your pass on the instrumented code.
opt -load-pass-plugin="${PATH2LIB}" -passes="${PASS}" ${BENCH}.profdata.bc -o ${BENCH}.bc

# Remove the build directory
rm -r "${LLVM_DIR}/build"

