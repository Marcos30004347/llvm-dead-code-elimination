cmake_minimum_required (VERSION 3.0)

project (RangeAnalysis)

set (CMAKE_CXX_STANDARD 11)

find_package(LLVM 8 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

execute_process (COMMAND llvm-config --cxxflags --ldflags OUTPUT_VARIABLE LLVM_CONFIG_N)

include_directories(${LLVM_INCLUDE_DIRS})

string (REPLACE "\n" " " LLVM_CONFIG "${LLVM_CONFIG_N}")

set (CMAKE_CXX_FLAGS "-O3 -g ${LLVM_CONFIG} -undefined dynamic_lookup")

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_library (RangeAnalysis SHARED
    #range analysis
    RangeAnalysis/RangeAnalysis.cpp
    RangeAnalysis/vSSA.cpp
    RangeAnalysis/ClientRA.cpp

    #dead code elimination
    DeadCodeElimination/DeadCodeElimination.cpp
)
