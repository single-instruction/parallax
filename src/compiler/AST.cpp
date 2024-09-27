#include <iostream>
#include "AST.h"
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

llvm::Type* CodeGenContext::getLLVMType(Type type) {
    switch (type) {
        case Type::I8: return llvm::Type::getInt8Ty(context);
        case Type::I16: return llvm::Type::getInt16Ty(context);
        case Type::I32: return llvm::Type::getInt32Ty(context);
        case Type::I64: return llvm::Type::getInt64Ty(context);
        case Type::U8: return llvm::Type::getInt8Ty(context);
        case Type::U16: return llvm::Type::getInt16Ty(context);
        case Type::U32: return llvm::Type::getInt32Ty(context);
        case Type::U64: return llvm::Type::getInt64Ty(context);
        case Type::F32: return llvm::Type::getFloatTy(context);
        case Type::F64: return llvm::Type::getDoubleTy(context);
        // case Type::String: return llvm::Type::(context);
        default: return nullptr;
    }
}

void NumberExprAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "NumberExpr: " << val << std::endl;
}

llvm::Value* NumberExprAST::codegen(CodeGenContext& context) {
    return llvm::ConstantFP::get(context.context, llvm::APFloat(val));
}

void VariableExprAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "VarExpr: " << name << std::endl;
}

llvm::Value* VariableExprAST::codegen(CodeGenContext& context) {
    llvm::Value* V = context.namedValues[name];
    // if (!V)
    //     throw std::runtime_error("Unknown variable name: " + name);
    return context.builder.CreateLoad(V->getType(), V, name.c_str());
}

void BinaryExprAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "BinaryExpr: " << op << std::endl;
    lhs->dump(indent + 2);
    rhs->dump(indent + 2);
}

llvm::Value* BinaryExprAST::codegen(CodeGenContext& context) {
    llvm::Value* L = lhs->codegen(context);
    llvm::Value* R = rhs->codegen(context);
    if (!L || !R)
        return nullptr;

    switch (op) {
        case '+': return context.builder.CreateFAdd(L, R, "addtmp");
        case '-': return context.builder.CreateFSub(L, R, "subtmp");
        case '*': return context.builder.CreateFMul(L, R, "multmp");
        case '/': return context.builder.CreateFDiv(L, R, "divtmp");
        default: std::cout << "Invalid binary operator" << std::endl;
    }

    return nullptr;
}

// Type BinaryExprAST::getType() const {
//     Type lhsType = lhs->getType();
//     Type rhsType = rhs->getType();
//     if (lhsType != rhsType)
//         throw std::runtime_error("Type mismatch in binary expression");
//     return lhsType;
// }

void VarDeclExprAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "VarDecl: " << name << std::endl;
    initVal->dump(indent + 2);
}

llvm::Value* VarDeclExprAST::codegen(CodeGenContext& context) {
    llvm::Function* TheFunction = context.builder.GetInsertBlock()->getParent();
    llvm::AllocaInst* Alloca = context.builder.CreateAlloca(context.getLLVMType(type), nullptr, name);
    
    llvm::Value* InitVal = initVal->codegen(context);
    if (!InitVal)
        return nullptr;

    context.builder.CreateStore(InitVal, Alloca);
    context.namedValues[name] = Alloca;
    return Alloca;
}

void PrintExprAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "Print:" << std::endl;
    arg->dump(indent + 2);
}

llvm::Value* PrintExprAST::codegen(CodeGenContext& context) {
    llvm::Value* ArgV = arg->codegen(context);
    if (!ArgV)
        return nullptr;

    llvm::Function* PrintfFunc = context.module->getFunction("printf");
    if (!PrintfFunc) {
        llvm::FunctionType* PrintfType = llvm::FunctionType::get(
            llvm::IntegerType::getInt32Ty(context.context),
            llvm::PointerType::get(llvm::Type::getInt8Ty(context.context), 0),
            true);
        PrintfFunc = llvm::Function::Create(PrintfType, llvm::Function::ExternalLinkage, "printf", context.module.get());
    }

    std::vector<llvm::Value*> ArgsV;
    ArgsV.push_back(context.builder.CreateGlobalString("%f\n", "printf_format"));
    ArgsV.push_back(ArgV);
    return context.builder.CreateCall(PrintfFunc, ArgsV, "printfcall");
}

void ReturnExprAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "Return:" << std::endl;
    expr->dump(indent + 2);
}

llvm::Value* ReturnExprAST::codegen(CodeGenContext& context) {
    llvm::Value* RetVal = expr->codegen(context);
    if (!RetVal)
        return nullptr;
    return context.builder.CreateRet(RetVal);
}

void FunctionAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "Function: " << name << std::endl;
    for (const auto& expr : body) {
        expr->dump(indent + 2);
    }
}

llvm::Function* FunctionAST::codegen(CodeGenContext& context) {
    std::vector<llvm::Type*> ArgTypes;
    for (const auto& arg : args) {
        ArgTypes.push_back(context.getLLVMType(arg.second));
    }

    llvm::FunctionType* FT = llvm::FunctionType::get(context.getLLVMType(returnType), ArgTypes, false);
    llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, context.module.get());

    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
        Arg.setName(args[Idx++].first);
    }

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(context.context, "entry", F);
    context.builder.SetInsertPoint(BB);

    context.namedValues.clear();
    for (auto &Arg : F->args()) {
        llvm::AllocaInst* Alloca = context.builder.CreateAlloca(Arg.getType(), nullptr, std::string(Arg.getName()));
        context.builder.CreateStore(&Arg, Alloca);
        context.namedValues[std::string(Arg.getName())] = Alloca;
    }

    for (const auto& expr : body) {
        expr->codegen(context);
    }

    llvm::verifyFunction(*F);
    return F;
}

void ModuleAST::dump(int indent) const {
    std::cout << std::string(indent, ' ') << "Module:" << std::endl;
    for (const auto& function : functions) {
        function->dump(indent + 2);
    }
}

void ModuleAST::codegen(CodeGenContext& context) {
    for (const auto& function : functions) {
        function->codegen(context);
    }
}
