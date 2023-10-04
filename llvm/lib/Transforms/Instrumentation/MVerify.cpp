#include "llvm/Transforms/Instrumentation/MVerify.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Instrumentation/AddressSanitizer.h"
#include "llvm/Transforms/Instrumentation/AddressSanitizerCommon.h"
#include <memory>

using namespace llvm;

PreservedAnalyses MVerifyPass::run(Function &F, FunctionAnalysisManager &FAM) {

    if (F.getName() != "main") {
        return PreservedAnalyses::all();
    }

    for (auto &BB : F) {
        errs() << "inserting instrumentation" << "\n";
        IRBuilder<> Builder(&*BB.getFirstInsertionPt());
        Module *M = F.getParent();
        Function *F = M->getFunction("_Z7mverifyv");
        std::vector<Value *> Args;
        Builder.CreateCall(F, Args);
        return PreservedAnalyses::all();
    }

    return PreservedAnalyses::all();
}


