#include "DeadCodeElimination.h"
#include "RangeAnalysis/RangeAnalysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include <iterator>

using namespace llvm;

void DeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
  AU.addRequired<InterProceduralRA<Cousot>>();
}

void DeadCodeElimination::visitBranch(BranchInst *inst) {
  if (inst->isConditional()) {
		CmpInst* cond = dyn_cast<CmpInst>(inst->getCondition());

		visitCmp(cond);
		
		// Range range_op0 =
    //     getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(0));

    // Range range_op1 =
    //     getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(1));

    // if (inst->getOperand(0)->hasName()) {
    //   errs() << inst->getOperand(0)->getName().str() << " - ";
    // }
		
		// //errs() << range_op0.getLower().bitsToDouble() << "\n";

		// range_op0.print(errs());

    // errs() << "\n";

    // if (inst->getOperand(1)->hasName()) {
    //   errs() << inst->getOperand(1)->getName().str() << " - ";
    // }

    // range_op1.print(errs());

    // errs() << "\n";
    // visitBlock(inst->getSuccessor(0));
    // visitBlock(inst->getSuccessor(1));
  }
}

void DeadCodeElimination::visitCmp(CmpInst *inst) {
  Range range_op0 = getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(0));
  Range range_op1 = getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(1));

	if(inst->getOperand(0)->hasName()) {
		errs() << inst->getOperand(0)->getName().str() << " - ";
	}

	range_op0.print(errs());
	errs() << "\n";

	if(inst->getOperand(1)->hasName()) {
		errs() << inst->getOperand(1)->getName().str() << " - ";
	}
	range_op1.print(errs());
	errs() << "\n";
	
}

void DeadCodeElimination::visitCall(CallInst *inst) {}
void DeadCodeElimination::visitLoad(LoadInst *inst) {}
void DeadCodeElimination::visitSelect(SelectInst *inst) {}
void DeadCodeElimination::visitBinary(BinaryOperator *inst) {}
void DeadCodeElimination::visitConstant(ConstantInt *intr) {}
void DeadCodeElimination::visitPhiNode(PHINode *inst) {}

void DeadCodeElimination::visitInstruction(Instruction *inst) {
  if (BranchInst *branch = dyn_cast<BranchInst>(inst)) {
    return visitBranch(branch);
  }
	
  if (CmpInst *cmp = dyn_cast<CmpInst>(inst)) {
    return visitCmp(cmp);
  }
}

void DeadCodeElimination::visitBlock(BasicBlock *BB) {
  typedef BasicBlock::InstListType::iterator inst_it;

  for (inst_it it = BB->getInstList().begin(); it != BB->getInstList().end();
       ++it) {
    // Get instruction from iterator
    Instruction *I = &*it;

    visitInstruction(I);
  }
}

bool DeadCodeElimination::runOnFunction(Function &F) {
	for(BasicBlock& b : F.getBasicBlockList()) {
		visitBlock(&b);
	}

  return false;
}
