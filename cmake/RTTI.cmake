set(CLANG_DISABLED_WARNINGS 
    "-Wno-misleading-indentation -Wno-unused-variable -Wno-narrowing \
    -Wno-unused-function -Wno-macro-redefined -Wno-unused-value \
    -Wno-infinite-recursion -Wno-missing-braces \
    -Wno-macro-redefined -Wno-uninitialized -Wno-address-of-temporary \
    -Wno-c++11-narrowing -Wno-unknown-attributes -Wno-c++11-narrowing \
    -Wpointer-bool-conversion -Wimplicit-int-float-conversion \
    -Wno-tautological-pointer-compare -Wno-delete-abstract-non-virtual-dtor \
    -Wno-unused-label -Wno-zero-as-null-pointer-constant -Wno-extra-semi-stmt \
    -Wno-missing-prototypes -Wno-old-style-cast -Wno-reorder-ctor \
    -Wno-implicit-int-float-conversion -Wno-c++98-compat -Wno-format"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_DISABLED_WARNINGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_DISABLED_WARNINGS}")
endif()

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti")
endif()