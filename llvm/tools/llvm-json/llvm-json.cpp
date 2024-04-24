#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ModuleSummaryIndex.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/SourceMgr.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
using namespace llvm;

extern "C" LLVMMemoryBufferRef LLVMCreateMemoryBufferWithMemoryRange(
    const char *InputData, size_t InputDataLength, const char *BufferName,
    LLVMBool RequiresNullTerminator); // FIXME: figure out how to include
                                      // "llvm-c/Core.h"

extern "C" LLVMBool
LLVMParseIRInContext(LLVMContextRef ContextRef, LLVMMemoryBufferRef MemBuf,
                     LLVMModuleRef *OutM,
                     char **OutMessage); // FIXME: figure out how to include
                                         // "llvm-c/Core.h"

int main(int argc, char **argv) {
  LLVMContext Context;
  LLVMContextRef ContextRef = (LLVMContextRef)&Context;

  // Parse the file now...
  SMDiagnostic Err;
  auto SetDataLayout = [](StringRef, StringRef) -> std::optional<std::string> {
    return std::nullopt;
  };
  ParsedModuleAndIndex ModuleAndIndex =
      parseAssemblyFileWithIndex("-", Err, Context, nullptr, SetDataLayout);
  std::unique_ptr<Module> M = std::move(ModuleAndIndex.Mod);
  if (!M.get()) {
    Err.print(argv[0], errs());
    return 1;
  }

  std::unique_ptr<ModuleSummaryIndex> Index = std::move(ModuleAndIndex.Index);

  errs() << M.get()->json().dump(2) << "\n";
  if (Index.get() && Index->begin() != Index->end())
    Index->print(errs());

  // LLVMModuleRef Mod = *M.get();
  LLVMModuleRef Mod;
  const char Data[] = "";
  LLVMMemoryBufferRef Buf =
      LLVMCreateMemoryBufferWithMemoryRange(Data, strlen(Data), "", 1);
  // LLVMContextRef context_ref;
  LLVMParseIRInContext(ContextRef, Buf, &Mod, nullptr);

  printf("\nBLANK LINE\n");
  errs() << ((Module *)Mod)->json().dump(2) << "\n";

  return 0;
}
