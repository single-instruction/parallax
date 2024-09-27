#include <iostream>
#include "AST.h"
#include "CodeGenContext.h"
#include "llvm/Support/CodeGen.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/IR/LegacyPassManager.h>

extern int yyparse();
extern FILE* yyin;
// TODO: Extend to support multiplt modules/files.
extern std::unique_ptr<ModuleAST> moduleAST;

int main(int argc, char** argv) {
    // TODO: Add support for various compiler flags [RFC]
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Open the input file
    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        std::cerr << "Error: Unable to open input file " << argv[1] << std::endl;
        return 1;
    }

    // Set Flex to read from it instead of defaulting to STDIN
    yyin = inputFile;
    
    // Parse through the input until there is no more:
    do {
        yyparse();
    } while (!feof(yyin));

    if (moduleAST) {
        std::cout << "Parsing successful!" << std::endl;
        moduleAST->dump();

        // TODO: Proper Type Checking
        // std::string typeCheckSuccess = moduleAST->typeCheck();
        // if (typeCheckSuccess.empty()) {
        //     std::cerr << "Type checking failed." << std::endl;
        //     return 1;
        // }
        // std::cout << "Type checking successful!" << std::endl;

        // // Code generation
        // CodeGenContext context;
        // moduleAST->codegen(context);

        // auto targetTriple = llvm::sys::getDefaultTargetTriple();
        // llvm::InitializeAllTargetInfos();
        // llvm::InitializeAllTargets();
        // llvm::InitializeAllTargetMCs();
        // llvm::InitializeAllAsmParsers();
        // llvm::InitializeAllAsmPrinters();
        // context.module->setTargetTriple(targetTriple);

        // std::string error;
        // auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

        // if (!target) {
        //     llvm::errs() << error;
        //     return 1;
        // }

        // auto CPU = "generic";
        // auto features = "";

        // llvm::TargetOptions opt;
        // auto RM = std::optional<llvm::Reloc::Model>();
        // auto targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, RM);

        // context.module->setDataLayout(targetMachine->createDataLayout());

        // auto filename = "output.o";
        // std::error_code EC;
        // llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OF_None);

        // if (EC) {
        //     llvm::errs() << "Could not open file: " << EC.message();
        //     return 1;
        // }

        // // New LLVM PassManager 
        // // Look into https://llvm.org/doxygen/structllvm_1_1ExtraVectorPassManager.html
        // llvm::LoopAnalysisManager LAM;
        // llvm::FunctionAnalysisManager FAM;
        // llvm::CGSCCAnalysisManager CGAM;
        // llvm::ModuleAnalysisManager MAM;

        // llvm::PassBuilder PB(targetMachine);

        // // Register all the basic analyses with the managers
        // PB.registerModuleAnalyses(MAM);
        // PB.registerCGSCCAnalyses(CGAM);
        // PB.registerFunctionAnalyses(FAM);
        // PB.registerLoopAnalyses(LAM);
        // PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

        // // Create the pass manager
        // llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);
        // // Run the passes
        // MPM.run(*context.module, MAM);

        // // Output the IR
        // llvm::outs() << "Generated LLVM IR:\n";
        // context.module->print(llvm::outs(), nullptr);

        // // Generate object code
        // auto fileType = llvm::CodeGenFileType::ObjectFile;
        // if (targetMachine->addPassesToEmitFile(MPM, dest, nullptr, fileType)) {
        //     llvm::errs() << "TargetMachine can't emit a file of this type";
        //     return 1;
        // }

        // MPM.run(*context.module, MAM);
        // dest.flush();

        // std::cout << "Wrote " << filename << std::endl;
    } else {
        std::cerr << "Error: AST not generated." << std::endl;
    }

    return 0;
}
