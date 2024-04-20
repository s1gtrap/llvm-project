find_package(nlohmann_json 3.2.0 REQUIRED)

set(LLVM_LINK_COMPONENTS AsmParser)

add_llvm_tool(llvm-json llvm-json.cpp DEPENDS intrinsics_gen)
