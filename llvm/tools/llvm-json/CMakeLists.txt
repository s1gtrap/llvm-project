find_package(nlohmann_json 3.2.0 REQUIRED)

set(LLVM_LINK_COMPONENTS AsmParser Core IRReader Support)

set(CMAKE_CXX_FLAGS "-s TOTAL_MEMORY=4096MB")

add_llvm_tool(llvm-json llvm-json.cpp DEPENDS intrinsics_gen)
