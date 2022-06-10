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

#include <iterator>

using namespace llvm;

void DeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
  AU.addRequired<InterProceduralRA<Cousot>>();
}

void DeadCodeElimination::visitBranch(BranchInst *inst) {
  if (inst->isConditional()) {
		CmpInst* cond = dyn_cast<CmpInst>(inst->getCondition());

		int cmpAnalysis = visitCmp(cond);

    errs() << cmpAnalysis << "\n";

    // if (cmpAnalysis != -1) {
    //   BranchInst* New = BranchInst::Create(inst->getSuccessor(cmpAnalysis));
    //   ICmpInst *cond = dyn_cast<ICmpInst>(inst->getCondition());
    //   ReplaceInstWithInst(inst, New);
    //   RecursivelyDeleteTriviallyDeadInstructions(cond);
    // }
		
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
	// TODO: in this function we should get the type of comparison (this github repo
	// lines 335 829 may help https://github.com/SandroMaglione/range-analysis-llvm/blob/master/BranchRange.cpp)
	// and then verify if one of the branches will never be taken, for instance i > 100,
	// if the range of i is [0, 99], the true branch will never be taken!.

  Range range_op0 = getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(0));
  Range range_op1 = getAnalysis<InterProceduralRA<Cousot>>().getRange(inst->getOperand(1));
	
  return getCmpAnalysis(range_op0, range_op1, inst->getPredicate());

  
	// if(inst->getOperand(0)->hasName()) {
	// 	errs() << inst->getOperand(0)->getName().str() << " - ";
	// }

	// range_op0.print(errs());
	// errs() << "\n";

	// if(inst->getOperand(1)->hasName()) {
	// 	errs() << inst->getOperand(1)->getName().str() << " - ";
	// }
	// range_op1.print(errs());
	// errs() << "\n";
	
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
