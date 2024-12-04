/* Copyright (C) 2018, Derrick Greenspan and 		*
 * the University of Central Florida 			*
 * Licensed under the terms of the MIT License. 	*
 * Please see the LICENSE file for more information 	 */

/*  Skeleton Author: Adrian Sampson 			*
 * Portions (C) 2015 and Licensed under the MIT License */

#include "llvm/IR/PassManager.h"
#include <cstdio>
#include <llvm-c/Core.h>
#include <llvm/Analysis/BlockFrequencyInfo.h>
#include <llvm/Analysis/BranchProbabilityInfo.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/MemoryLocation.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/IndVarSimplify.h>
#include <llvm/Transforms/Scalar/LoopPassManager.h>
#include <llvm/Transforms/Scalar/LoopRotation.h>
#include <llvm/Transforms/Scalar/LoopUnrollPass.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/LoopUtils.h>
#include <unordered_map>
#include <unordered_set>
using namespace llvm;

namespace {
int iterator = 0;
struct funcValue {
  Value *func;
  Value *memValue;
  int uses = 0;
  bool visited = false;
  int id;
  IRBuilderBase::InsertPoint IP;
  Instruction *declaredOp;
  BasicBlock *BB;
};
int findFunc(funcValue *ptr, Value *func);

Instruction *iterateInsertPoints(funcValue *ptr, int position) {
  return ptr[position].declaredOp;
}

funcValue *addFunc(funcValue *ptr, Value *func, Instruction *declaredOp,
                   BasicBlock *basic) {
  funcValue *retFunc =
      (funcValue *)realloc(ptr, sizeof(funcValue) * (iterator + 1));
  retFunc[iterator].func = func;
  retFunc[iterator].declaredOp = declaredOp;
  retFunc[iterator].uses = 0;
  retFunc[iterator].id = iterator;
  retFunc[iterator].BB = basic;
  iterator++;

  return retFunc;
}

bool isInUsers(Instruction *Bi, Instruction *whichOne, Value *PtrOp) {
  /*  If Instruction is part of the user list of vals */
  for (auto U : PtrOp->users()) {
    if (auto *I = dyn_cast<Instruction>(U)) {
      if (I == Bi)
        return true;
    }
  }
  return false;
}

int funcValExists(funcValue *ptr, Value *func) {
  int ret = findFunc(ptr, func);
  return ret;
}

int findFunc(funcValue *ptr, Value *func) {
  int i;
  Value *compFunc = func;
  for (i = 0; i < iterator; i++) {

    if (isa<BitCastInst>(func))
      compFunc = cast<BitCastInst>(func)->getOperand(0);

    if (ptr[i].func == compFunc) {
      return i;
    }
  }
  /*  Value was not found */
  return -1;
}

class llamaPass : public PassInfoMixin<llamaPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    BranchProbabilityInfo &BPI = FAM.getResult<BranchProbabilityAnalysis>(F);
    BlockFrequencyAnalysis::Result &BFI =
        FAM.getResult<BlockFrequencyAnalysis>(F);
    LLVMContext &context = F.getContext();
    funcValue *values = (funcValue *)calloc(sizeof(funcValue), 1);
    funcValue value;
    IRBuilder<> builder(context);
    Instruction *theOp;

    // Collect frequent paths
    std::unordered_set<BasicBlock *> FrequentPaths;
    std::unordered_map<BasicBlock *, float> BranchFrequencies;

    for (auto &BB : F) {
      if (auto *BI = dyn_cast<BranchInst>(BB.getTerminator())) {
        for (unsigned i = 0; i < BI->getNumSuccessors(); ++i) {
          BasicBlock *Succ = BI->getSuccessor(i);
          BranchProbability Prob = BPI.getEdgeProbability(&BB, Succ);
          if (Prob >= BranchProbability::getBranchProbability(8, 10)) {
            FrequentPaths.insert(Succ);
          }
          /*errs() << static_cast<float>(Prob.getNumerator()) /
                        Prob.getDenominator()
                 << "\n";
          BranchFrequencies[Succ] +=
              static_cast<float>(Prob.getNumerator()) / Prob.getDenominator();*/
        }
      }
    }
    /*
    // Calculate block profile count
    uint64_t total_block_count = 0;
    std::unordered_map<BasicBlock *, uint64_t> block_count;
    for (auto &BB : F) {
      uint64_t bb_profile_count =
          BFI.getBlockProfileCount(&BB, false).value();
      total_block_count += bb_profile_count;
      //block_count[&BB] = bb_profile_count;
      errs() << (bb_profile_count) << "\n";
    }*/

    for (auto &B : F) {
      for (auto &I : B) {
        auto op = &I;
        theOp = op;
        Value *retValue;
        if (isa<CallInst>(I)) {
          if ((cast<CallInst>(I).getCalledFunction()->getName() == "calloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() == "malloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() == "realloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() ==
               "_mm_malloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() ==
               "_internal_calloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() ==
               "_internal_malloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() ==
               "_internal_realloc") ||
              (cast<CallInst>(I).getCalledFunction()->getName() ==
               "_internal__mm_malloc")) {
            IRBuilder<> builder(op);
            /*  For each allocation via calloc, malloc, realloc, get the
             *  insert point, the value (for the store ptr later),  */
            values = addFunc(values, &(cast<Value>(I)), op, &B);
            errs() << ("ere\n");
          }
        } /* After a call inst, a store inst is performed. */
        if (isa<StoreInst>(I)) {
          Value *ValOp = cast<StoreInst>(I).getValueOperand();
          Value *ptrOp = cast<StoreInst>(I).getPointerOperand();

          int pos = funcValExists(values, ValOp);
          int val = 0;
          if (pos != -1) {
            for (auto U : ptrOp->users()) {
              values[pos].uses++;
              if (auto *I = dyn_cast<Instruction>(U)) {
                int differential = 0, loopDepth = 0;
                BasicBlock *BB;
                if (int depth = LI.getLoopDepth(I->getParent())) {
                  loopDepth = depth;
                  differential = 15;
                  bool activate = false;
                  BB = I->getParent();
                  for (auto &in : *BB) {
                    if (!activate && in.isIdenticalTo(I))
                      activate = true;
                    else if (activate && in.isIdenticalTo(I))
                      activate = false;
                    else if (activate) {
                      differential--;
                    }
                  }
                  int not_used = 0, used = 0;
                }
                val += pow(std::max(differential, 0), loopDepth * 2);
              }
            }

            /*  If we allocated the value referenced by the operand via
             * calloc, malloc, or realloc, count the number of times the
             * pointer operand is used */
            values[pos].uses += val;
          }
        }
      }
    }

    for (int i = 0; i < iterator; i++) {
		if(FrequentPaths.find(values[i].BB) != FrequentPaths.end()){
                  values[i].uses *= 100;
                }

      // values[i].uses *= float(block_count[values[i].BB]) / total_block_count;
    }

    errs() << "ITERATOR " << iterator << "\n";
    for (int i = 0; i < 2; i++) {
      errs() << i << '\n';
      errs() << "printing use" << (values[i].uses) << "\n";
      ;
    }

    /*
   Now we insert the uses..

  int i;
  for (i = 0; i < iterator; i++) {

    fprintf(stderr, "Site: %d\n", values[i].id);
    IRBuilder<> builder(theOp);
    builder.SetInsertPoint(values[i].declaredOp);
    std::vector<Type *> paramTypes = {Type::getInt64Ty(context),
                                      Type::getInt32Ty(context)};
    Type *retType = Type::getVoidTy(context);
    FunctionType *LLVMScoreType =
        FunctionType::get(retType, paramTypes, false);
    FunctionCallee LLVMScoreFuncCallee =
        F.getParent()->getOrInsertFunction("setLLVMScore", LLVMScoreType);
    Function *LLVMScoreFunc =
        llvm::cast<Function>(LLVMScoreFuncCallee.getCallee());
    Value *args_LLVMScore[] = {
        ConstantInt::get(Type::getInt64Ty(context), values[i].uses),
        ConstantInt::get(Type::getInt32Ty(context), i)};
    builder.CreateCall(LLVMScoreFunc, args_LLVMScore);
  }
  iterator = 0;
  free(values);
  values = (funcValue *)calloc(sizeof(funcValue), 1);
}
*/
    return PreservedAnalyses::all();
  }
};
} // namespace
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "llamaPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "llama-pass") {
                    FPM.addPass(llamaPass());
                    return true;
                  }
                  return false;
                });
          }};
}