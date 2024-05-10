find_package(nlohmann_json 3.2.0 REQUIRED)

set(LLVM_LINK_COMPONENTS AsmParser Core IRReader Support)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sTOTAL_MEMORY=2048MB")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sTOTAL_MEMORY=2048MB")

add_llvm_tool(llvm-json llvm-json.cpp DEPENDS intrinsics_gen)
