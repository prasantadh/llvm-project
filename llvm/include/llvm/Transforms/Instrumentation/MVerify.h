#ifndef LLVM_TRANSFORMS_INSTRUMENTATION_MVERIFY_H
#define LLVM_TRANSFORMS_INSTRUMENTATION_MVERIFY_H

#include "llvm/IR/PassManager.h"
namespace llvm {

class MVerifyPass : public PassInfoMixin<MVerifyPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
};

} // end namespace llvm

#endif
