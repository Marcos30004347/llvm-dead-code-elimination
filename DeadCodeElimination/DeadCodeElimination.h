#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/PassAnalysisSupport.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "RangeAnalysis/RangeAnalysis.h"

using namespace llvm;

struct DeadCodeElimination : public FunctionPass {
  static char ID;

  DeadCodeElimination() : FunctionPass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override;
  bool runOnFunction(Function &F) override;
 
  void visitBranch(BranchInst *inst);
  int visitCmp(CmpInst *inst);
  void visitCall(CallInst *inst);
  void visitLoad(LoadInst *inst);
  void visitSelect(SelectInst *inst);
  void visitBinary(BinaryOperator *inst);
  void visitConstant(ConstantInt *intr);
  void visitPhiNode(PHINode *inst);
  void visitInstruction(Instruction *inst);
  void visitBlock(BasicBlock *BB);
};

char DeadCodeElimination::ID = 0;


static RegisterPass<DeadCodeElimination> X("dead-code-elimination",
                                           "Dead Code Elimination Pass",
                                           false /* Only looks at CFG */,
                                           false /* Analysis Pass */
);

static void registerDeadCodeEliminationPass(const PassManagerBuilder &,
                                            legacy::PassManagerBase &PM) {
  PM.add(new InterProceduralRA<CropDFS>());
  PM.add(new DeadCodeElimination());
}

static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible,
                                registerDeadCodeEliminationPass);
