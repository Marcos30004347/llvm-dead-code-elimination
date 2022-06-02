all: env
	cmake --build build/

examples-ir:
	/opt/local/libexec/llvm-8.0/bin/clang -Xclang -disable-O0-optnone ./examples/program0.cpp -o ./examples/example0.ll -c -emit-llvm
	/opt/local/libexec/llvm-8.0/bin/clang -S -emit-llvm -o ./examples/example1.ll ./examples/program1.cpp

run-range-analysis:
	/opt/local/libexec/llvm-8.0/bin/opt -instnamer -mem2reg -break-crit-edges ./examples/example0.ll -S -o example.ll
	/opt/local/libexec/llvm-8.0/bin/opt -load ./build/libRangeAnalysis.dylib -vssa -client-ra -dead-code-elimination -o=example.ll < ./example.ll

env:
	cmake -DLLVM_ENABLE_STATS=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DLLVM_INSTALL_DIR=/opt/local/libexec/llvm-8.0 -G "Unix Makefiles" -B build/
	if [ -f "./compile_commands.json" ]; then rm -rf ./compile_commands.json; fi
	ln ./build/compile_commands.json .

clean:
	rm -rf build compile_commands.json
