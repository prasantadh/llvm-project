#include "llvm/Transforms/Instrumentation/MVerify.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Instrumentation/AddressSanitizer.h"
#include "llvm/Transforms/Instrumentation/AddressSanitizerCommon.h"
#include <memory>
#include <vector>

using namespace llvm;

bool MVerifyPass::getInterestingMemoryOperands(Instruction *I) {

  bool Ans = false;
  if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
    errs() << "found memory operand order 1\n";
  } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
    errs() << "found memory operand order 2\n";
    Ans = true;
  } else if (AtomicRMWInst *RMW = dyn_cast<AtomicRMWInst>(I)) {
    errs() << "found memory operand order 3\n";
  } else if (AtomicCmpXchgInst *XCHG = dyn_cast<AtomicCmpXchgInst>(I)) {
    errs() << "found memory operand order 4\n";
  } else if (auto *CI = dyn_cast<CallInst>(I)) {
    if (CI->getIntrinsicID() == Intrinsic::masked_load ||
        CI->getIntrinsicID() == Intrinsic::masked_store) {
       errs() << "found memory operand order 5\n";
    } else {
      for (unsigned ArgNo = 0; ArgNo < CI->arg_size(); ArgNo++) {
        errs() << "found memory operand order 6\n";
      }
    }
  }
  return Ans;
}

PreservedAnalyses MVerifyPass::run(Function &F, FunctionAnalysisManager &FAM) {

    if (F.getName() != "main") {
        errs() << F.getName() << "\n";
        return PreservedAnalyses::all();
    }
    // TODO
    // if __attribute__((no_sanitize("mverify"))), skip
    // if in ignorelist, skip
    // apply this to at least mverify function so we don't recurse forever

    LLVMContext &Context = F.getContext();

    for (auto &BB: F) {
        for (auto &I: BB) {
            if (getInterestingMemoryOperands(&I)) {
                errs() << I << "\n";
                IRBuilder<> Builder(&I);
                Module *M = F.getParent();
                StoreInst *SI = dyn_cast<StoreInst>(&I);

                Value *V = Builder.CreatePointerCast(SI->getPointerOperand(), PointerType::getInt8PtrTy(Context, true));
                FunctionType *CalleeType = FunctionType::get(
                        Type::getVoidTy(Context),
                        PointerType::getInt8PtrTy(Context, true),
                        false
                        );
                FunctionCallee Callee = M->getOrInsertFunction("_Z7mverifyPv", CalleeType);
                Builder.CreateCall(Callee, {V});
            }
        }
    } 

    return PreservedAnalyses::all();
}


