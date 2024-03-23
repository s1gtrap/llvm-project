set(LLVM_LINK_COMPONENTS
  AsmParser
)

add_llvm_library(
  LLVMJs
  Lib.cpp
  Lib.h
)
