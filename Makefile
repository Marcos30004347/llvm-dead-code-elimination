all: env
	cmake --build build/

examples-ir:
	/home/luizcouto/Apps/llvm-project-llvmorg-8.0.1/build/bin/clang -Xclang -disable-O0-optnone ./examples/program0.cpp -o ./examples/example0.ll -c -emit-llvm
	/home/luizcouto/Apps/llvm-project-llvmorg-8.0.1/build/bin/clang -S -emit-llvm -o ./examples/example1.ll ./examples/program1.cpp

run-range-analysis:
	/home/luizcouto/Apps/llvm-project-llvmorg-8.0.1/build/bin/opt -instnamer -mem2reg -break-crit-edges ./examples/example0.ll -S -o example.ll
	/home/luizcouto/Apps/llvm-project-llvmorg-8.0.1/build/bin/opt -stats -S -load ./build/libRangeAnalysis.so -vssa -dead-code-elimination -o=example.ll < ./example.ll

env:
	cmake -DLLVM_ENABLE_STATS=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DLLVM_INSTALL_DIR=/home/luizcouto/Apps/llvm-project-llvmorg-8.0.1 -G "Unix Makefiles" -B build/
	if [ -f "./compile_commands.json" ]; then rm -rf ./compile_commands.json; fi
	ln ./build/compile_commands.json .

clean:
	rm -rf build compile_commands.json
