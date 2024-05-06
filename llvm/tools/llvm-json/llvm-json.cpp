#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ModuleSummaryIndex.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/SourceMgr.h"
#include <cstdint>
#include <emscripten/emscripten.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
using namespace llvm;

extern "C" LLVMMemoryBufferRef LLVMCreateMemoryBufferWithMemoryRange(
    const char *InputData, size_t InputDataLength, const char *BufferName,
    LLVMBool RequiresNullTerminator); // FIXME: figure out how to include
                                      // "llvm-c/Core.h"

extern "C" LLVMBool LLVMParseIRInContext(
    LLVMContextRef ContextRef, LLVMMemoryBufferRef MemBuf, LLVMModuleRef *OutM,
    char **OutMessage); // FIXME: figure out how to include "llvm-c/Core.h"

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE int add(int a, int b) {
  emscripten_log(EM_LOG_INFO, "add(%i, %i)", a, b);
  return a + b;
}

EXTERN EMSCRIPTEN_KEEPALIVE int len(char *str) {
  emscripten_log(EM_LOG_INFO, "len(%p)", str);
  emscripten_log(EM_LOG_INFO, "len(\"%s\")", str);
  return strlen(str);
}

EXTERN EMSCRIPTEN_KEEPALIVE int size() { return sizeof(Module); }

EXTERN EMSCRIPTEN_KEEPALIVE int printarray(char *Data, int len) {
  emscripten_log(EM_LOG_INFO, "printarray(%i, %i)", Data, len);
  /*for (int i = 0; i < len; i++) {
    emscripten_log(EM_LOG_INFO, "Mod[%i] = %i", i, *(uint8_t *)(Data + i));
  }*/
  return (int)Data + len;
}

class Static {
public:
  LLVMModuleRef Mod;
  Static() { emscripten_log(EM_LOG_INFO, "construct Static()"); }
  ~Static() { emscripten_log(EM_LOG_INFO, "destructure Static()"); }
};

EXTERN EMSCRIPTEN_KEEPALIVE Static *S = nullptr;

EXTERN EMSCRIPTEN_KEEPALIVE std::shared_ptr<LLVMModuleRef> Mod =
    std::make_shared<LLVMModuleRef>();

EXTERN EMSCRIPTEN_KEEPALIVE LLVMModuleRef parse(char *Data) {
  LLVMContext Context;
  LLVMContextRef ContextRef = (LLVMContextRef)&Context;

  emscripten_log(EM_LOG_INFO, "parse(%p)", Data);
  emscripten_log(EM_LOG_INFO, "parse(\"%s\"), *Mod = %#x", Data, *Mod);

  LLVMMemoryBufferRef Buf =
      LLVMCreateMemoryBufferWithMemoryRange(Data, strlen(Data), "", 1);
  LLVMContextRef context_ref;
  LLVMParseIRInContext(ContextRef, Buf, Mod.get(), nullptr);

  emscripten_log(EM_LOG_INFO, "parse(%p)", Data);
  emscripten_log(EM_LOG_INFO, "parse(\"%s\"), *Mod = %#x", Data, *Mod);

  return *Mod;
}

int main(int argc, char **argv) { S = new Static(); }

/*int main(int argc, char **argv) {
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
}*/
