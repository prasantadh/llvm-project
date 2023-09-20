#include "llvm/Transforms/Instrumentation/MVerify.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

PreservedAnalyses MVerifyPass::run(Function &F, FunctionAnalysisManager &FAM) {
    errs() << F.getName() << "\n";
    return PreservedAnalyses::all();
}
