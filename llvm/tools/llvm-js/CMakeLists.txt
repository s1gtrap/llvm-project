set(LLVM_LINK_COMPONENTS
  AsmParser
  BitWriter
  Core
  Support
  )

add_llvm_tool(llvm-js
  llvm-js.cpp

  DEPENDS
  intrinsics_gen
  )
