cmake_minimum_required(VERSION 3.1)
project(meta)


find_package(LLVM REQUIRED CONFIG)

link_directories(${LLVM_LIB_DIR})


file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)


add_executable(meta ${sources} ${headers})


target_link_libraries(meta PUBLIC ${CLANG_LIBS} ${LLVM_LIBS_CORE} ${LLVM_LDFLAGS})
target_include_directories(meta PRIVATE ${LLVM_INCLUDE_DIR} ${CLANG_INCLUDE_DIRS})