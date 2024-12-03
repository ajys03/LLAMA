add_library(llamaPass MODULE
    llama.cpp
)

add_library(llvm_bug_fix SHARED
  llvm_bug_fix.c
)

####################
#       LLVM       #
####################
find_package(LLVM REQUIRED CONFIG)
target_include_directories(llamaPass PRIVATE ${LLVM_INCLUDE_DIR})
target_link_libraries(llamaPass PRIVATE LLVMCore LLVMSupport)

# Use C++17 to compile our pass (i.e., supply -std=c++17).
target_compile_features(llamaPass PRIVATE cxx_std_17)

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(llamaPass PROPERTIES
    COMPILE_FLAGS "-fno-rtti -g"
)

# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
if(APPLE)
    set_target_properties(llamaPass PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup"
    )
endif(APPLE)

set_target_properties(llamaPass PROPERTIES COMPILE_FLAGS "-D__GLIBCXX_USE_CXX11_ABI=0 -fno-rtti")
