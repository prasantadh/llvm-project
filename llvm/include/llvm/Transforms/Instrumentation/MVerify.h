#ifndef LLVM_TRANSFORMS_INSTRUMENTATION_MVERIFY_H
#define LLVM_TRANSFORMS_INSTRUMENTATION_MVERIFY_H

#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
namespace llvm {

class MVerifyPass : public PassInfoMixin<MVerifyPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);

    static bool isRequired() { return true; }
    bool getInterestingMemoryOperands(Instruction *I);
};

} // end namespace llvm

#endif
