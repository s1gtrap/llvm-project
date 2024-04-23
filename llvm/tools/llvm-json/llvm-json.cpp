//===--- llvm-as.cpp - The low-level LLVM assembler -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This utility may be invoked in the following manner:
//   llvm-as --help         - Output information about command line switches
//   llvm-as [options]      - Read LLVM asm from stdin, write bitcode to stdout
//   llvm-as [options] x.ll - Read LLVM asm from the x.ll file, write bitcode
//                            to the x.bc file.
//
//===----------------------------------------------------------------------===//

#include "llvm/AsmParser/Parser.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ModuleSummaryIndex.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/SystemUtils.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
using namespace llvm;

cl::OptionCategory AsCat("llvm-json Options");

static cl::opt<std::string> InputFilename(cl::Positional,
                                          cl::desc("<input .llvm file>"),
                                          cl::init("-"));

static cl::opt<std::string> OutputFilename("o",
                                           cl::desc("Override output filename"),
                                           cl::value_desc("filename"),
                                           cl::cat(AsCat));

static cl::opt<bool> Force("f", cl::desc("Enable binary output on terminals"),
                           cl::cat(AsCat));

static cl::opt<bool> DisableOutput("disable-output", cl::desc("Disable output"),
                                   cl::init(false), cl::cat(AsCat));

static cl::opt<bool> EmitModuleHash("module-hash", cl::desc("Emit module hash"),
                                    cl::init(false), cl::cat(AsCat));

extern bool WriteNewDbgInfoFormatToBitcode;

int main(int argc, char **argv) {
  InitLLVM X(argc, argv);
  cl::HideUnrelatedOptions(AsCat);
  cl::ParseCommandLineOptions(argc, argv, "llvm .ll -> .bc assembler\n");
  LLVMContext Context;

  // Parse the file now...
  SMDiagnostic Err;
  auto SetDataLayout = [](StringRef, StringRef) -> std::optional<std::string> {
    return std::nullopt;
  };
  ParsedModuleAndIndex ModuleAndIndex;
  ModuleAndIndex = parseAssemblyFileWithIndex(InputFilename, Err, Context,
                                              nullptr, SetDataLayout);
  std::unique_ptr<Module> M = std::move(ModuleAndIndex.Mod);
  if (!M.get()) {
    Err.print(argv[0], errs());
    return 1;
  }

  std::unique_ptr<ModuleSummaryIndex> Index = std::move(ModuleAndIndex.Index);

  errs() << M.get()->json().dump(2) << "\n";
  if (Index.get() && Index->begin() != Index->end())
    Index->print(errs());

  // if (!DisableOutput)
  //   WriteOutputFile(M.get(), Index.get());

  return 0;
}
