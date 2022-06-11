#include "DeadCodeElimination.h"
#include "RangeAnalysis/RangeAnalysis.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/ADT/Statistic.h"

#include <iterator>

#define DEBUG_TYPE "dead-code-elimination"

using namespace llvm;

void DeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
  AU.addRequired<InterProceduralRA<Cousot>>();
}

void DeadCodeElimination::visitBranch(BranchInst *inst) {
  if (inst->isConditional()) {
		CmpInst* cond = dyn_cast<CmpInst>(inst->getCondition());

		if(!cond) return;

		int cmp = visitCmp(cond);

    if (cmp != -1) {
      BasicBlock* succ = inst->getSuccessor(cmp);

			BranchInst* branch = BranchInst::Create(succ);

			ReplaceInstWithInst(inst, branch);

      RecursivelyDeleteTriviallyDeadInstructions(cond);
    }
  }
}

int getCmpAnalysis(Range &I, Range &J, ICmpInst::Predicate pred) {
  
  // i < j
  if (pred == ICmpInst::ICMP_SLT) {
    // In this first case, i is always greater or equal than j
    if (I.getLower().sge(J.getUpper())) return 1; // I.l >= J.u

    // In this second case, i is always lower than j
    if(I.getUpper().slt(J.getLower())) return 0; // I.u < J.l

  }

  if (pred == ICmpInst::ICMP_ULT) {
    // In this first case, i is always greater or equal than j
    if (I.getLower().uge(J.getUpper())) return 1; // I.l >= J.u

    // In this second case, i is always lower than j
    if(I.getUpper().ult(J.getLower())) return 0; // I.u < J.l
  
  }


  // i > j
  if (pred == ICmpInst::ICMP_SGT) {
    // In this first case, i is always lower or equal than j
    if (I.getUpper().sle(J.getLower())) return 1; // I.u <= J.l

    // In this second case, i is always greater than j
    if (I.getLower().sgt(J.getUpper())) return 0; // I.l > J.u

  }

  if (pred == ICmpInst::ICMP_UGT) {
    // In this first case, i is always lower or equal than j
    if (I.getUpper().ule(J.getLower())) return 1; // I.u <= J.l

    // In this second case, i is always greater than j
    if (I.getLower().ugt(J.getUpper())) return 0; // I.l > J.u
  }


  // i <= j
  if (pred == ICmpInst::ICMP_SLE) {
    // In this first case, i is always greater than j
    if (I.getLower().sgt(J.getUpper())) return 1; // I.l > J.u

    // In this second case, i is always lower or equal than j
    if (I.getUpper().sle(J.getLower())) return 0; // I.u <= J.l
  }

  if (pred == ICmpInst::ICMP_ULE) {
    // In this first case, i is always greater than j
    if (I.getLower().ugt(J.getUpper())) return 1; // I.l > J.u

    // In this second case, i is always lower or equal than j
    if (I.getUpper().ule(J.getLower())) return 0; // I.u <= J.l
  }


  // i >= j
  if (pred == ICmpInst::ICMP_SGE) {
    // In this first case, i is always lower than j
    if (I.getUpper().slt(J.getLower())) return 1; // I.u < J.l

    // In this second case, i is always greater or equal than j
    if (I.getLower().sge(J.getUpper())) return 0; // I.l >= J.u
  }

  if (pred == ICmpInst::ICMP_UGE) {
    // In this first case, i is always lower than j
    if (I.getUpper().ult(J.getLower())) return 1; // I.u < J.l

    // In this second case, i is always greater or equal than j
    if (I.getLower().uge(J.getUpper())) return 0; // I.l >= J.u
  }

  return -1;

}

int DeadCodeElimination::visitCmp(CmpInst *inst) {
  Range range_op0 = getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(0));
  Range range_op1 = getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(1));
	
  return getCmpAnalysis(range_op0, range_op1, inst->getPredicate());
}


bool DeadCodeElimination::runOnFunction(Function &F) {
	
  int BBcounterBefore = 0;
  int IcounterBefore = 0;

  for(BasicBlock &BB : F) {
    BBcounterBefore++;
    for (Instruction &I : BB) {  
      IcounterBefore++;
    }
  }
  
  for(BasicBlock& b : F.getBasicBlockList()) {
		if(BranchInst* br = dyn_cast<BranchInst>(b.getTerminator()))
			visitBranch(br);
	}

	removeUnreachableBlocks(F);

  int BBcounterAfter = 0;
  int IcounterAfter = 0;

  for(BasicBlock &BB : F) {
    BBcounterAfter++;
    for (Instruction &I : BB) {  
      IcounterAfter++;
    }
  }


  STATISTIC(InstructionsEliminated, "Number of instructions eliminated");
  STATISTIC(BasicBlocksEliminated,  "Number of basic blocks entirely eliminated");

  InstructionsEliminated = IcounterBefore - IcounterAfter;
  BasicBlocksEliminated = BBcounterBefore - BBcounterAfter;

  return false;
}
