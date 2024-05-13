add_subdirectory(nlohmann_json)

set(LLVM_LINK_COMPONENTS AsmParser Core IRReader Support)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sTOTAL_MEMORY=2048MB -sEXPORTED_FUNCTIONS=_parse -sASSERTIONS=1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sTOTAL_MEMORY=2048MB -sEXPORTED_FUNCTIONS=_parse -sASSERTIONS=1")

add_llvm_tool(llvm-json llvm-json.cpp DEPENDS intrinsics_gen)

target_link_options(llvm-json PRIVATE
        -sEXPORTED_FUNCTIONS=['_main','_parse']
        -sEXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'])
