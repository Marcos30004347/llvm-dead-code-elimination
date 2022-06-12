# LLVM Dead Code Eliminiation using Range Analysis

This project is a Dead Code Elimination Pass for LLVM that uses range analysis to statically decide if a code path is dead or not.

Requirements:
- LLVM 8.0
- CMake
- Clang

LLVM 8 is required since the Range Analysis Pass that we use also require the 8'th version of the framework.

## Build Instructions

To compile the pass, under the source tree of the project run:

```bash
	make
```

## Running the pass

Before running the pass using llvm optmizer, you should first generate a valid IR code, and example on how to generate such IR for and program.cpp c++ program is:

```bash
	 clang -Xclang -disable-O0-optnone ./examples/program.cpp -o ./examples/program.ll -c -emit-llvm
```

After building the project, you can run the pass with:

```bash
   $LLVM_8_PATH/bin/opt -instnamer -mem2reg -break-crit-edges program.ll -S -o program.ll
   $LLVM_8_PATH/bin/opt -stats -S -load ./build/libDeadCodeElimination.so -vssa -dead-code-elimination -o=example.ll < ./example.ll
```
