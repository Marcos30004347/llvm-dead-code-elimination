all: env
	cmake --build build/

env:
	cmake -G "Unix Makefiles" -B build/

clean:
	rm -rf build compile_commands.json