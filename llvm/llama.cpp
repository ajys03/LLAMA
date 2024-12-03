/* Copyright (C) 2018, Derrick Greenspan and
 * the University of Central Florida
 * Licensed under the terms of the MIT License.
 * Please see the LICENSE file for more information.
 *
 * Skeleton Author: Adrian Sampson
 * Portions (C) 2015 and Licensed under the MIT License*/

#include "llvm/IR/PassManager.h"
#include <llvm/Analysis/BlockFrequencyInfo.h>
#include <llvm/Analysis/BranchProbabilityInfo.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/LoopUtils.h>

using namespace llvm;

namespace {

struct FuncValue {
  Value *func;
  Value *memValue;
  int uses = 0;
  bool visited = false;
  int id;
  Instruction *declaredOp;
};

class LlamaPass : public PassInfoMixin<LlamaPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    BranchProbabilityInfo &BPI = FAM.getResult<BranchProbabilityAnalysis>(F);

    LLVMContext &Context = F.getContext();
    std::vector<FuncValue> values;
    IRBuilder<> Builder(Context);

    // Collect frequent paths
    std::set<BasicBlock *> FrequentPaths;
    std::map<BasicBlock *, float> BranchFrequencies;

    for (auto &BB : F) {
      if (auto *BI = dyn_cast<BranchInst>(BB.getTerminator())) {
        for (unsigned i = 0; i < BI->getNumSuccessors(); ++i) {
          BasicBlock *Succ = BI->getSuccessor(i);
          BranchProbability Prob = BPI.getEdgeProbability(&BB, Succ);
          if (Prob >= BranchProbability::getBranchProbability(8, 10)) {
            FrequentPaths.insert(Succ);
            BranchFrequencies[Succ] =
                static_cast<float>(Prob.getNumerator()) / Prob.getDenominator();
          }
        }
      }
    }

    // Analyze allocations and uses
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *Call = dyn_cast<CallInst>(&I)) {
          auto *CalledFunc = Call->getCalledFunction();
          if (CalledFunc &&
              (CalledFunc->getName().starts_with("_internal_malloc") ||
               CalledFunc->getName().starts_with("_internal_calloc") ||
               CalledFunc->getName().starts_with("_internal_realloc")||
               CalledFunc->getName().starts_with("_internal__mm_malloc"))) {
            values.push_back(
                {Call, nullptr, 0, false, static_cast<int>(values.size()), &I});
          }
        }

        if (auto *Store = dyn_cast<StoreInst>(&I)) {
          Value *StoredValue = Store->getValueOperand();
          Value *PointerOperand = Store->getPointerOperand();

          for (auto &Val : values) {
            if (Val.func == StoredValue) {
              Val.uses++;
              if (int Depth = LI.getLoopDepth(Store->getParent())) {
                Val.uses += 15 * Depth * Depth;
              }
              Val.uses *= BranchFrequencies[&BB];
            }
          }
        }
      }
    }

    // Insert calls to the scoring function
    for (auto &Val : values) {
      Builder.SetInsertPoint(Val.declaredOp);
      FunctionType *ScoreFuncType = FunctionType::get(
          Type::getVoidTy(Context),
          {Type::getInt64Ty(Context), Type::getInt32Ty(Context)}, false);
      FunctionCallee ScoreFunc =
          F.getParent()->getOrInsertFunction("setLLVMScore", ScoreFuncType);

      Value *Args[] = {ConstantInt::get(Type::getInt64Ty(Context), Val.uses),
                       ConstantInt::get(Type::getInt32Ty(Context), Val.id)};

      Builder.CreateCall(ScoreFunc, Args);
    }

    return PreservedAnalyses::all();
  }
};

} // namespace

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LlamaPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "llama-pass") {
                    FPM.addPass(LlamaPass());
                    return true;
                  }
                  return false;
                });
          }};
}