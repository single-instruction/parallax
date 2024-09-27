#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <map>
#include <iostream>
#include <stdexcept>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

struct Location {
    std::string file;
    int line;
    int col;

    std::string toString() const {
        return file + ":" + std::to_string(line) + ":" + std::to_string(col);
    }
};

enum class Type {
    I8, I16, I32, I64,
    U8, U16, U32, U64,
    F32, F64,
    String,
    Void
};

std::string typeToString(Type type);

class TypeCheckException : public std::runtime_error {
public:
    TypeCheckException(const std::string& message, const Location& loc)
        : std::runtime_error(loc.toString() + ": " + message) {}
};

class CodeGenContext {
public:
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> namedValues;

    CodeGenContext() : builder(context), module(std::make_unique<llvm::Module>("parallax module", context)) {}

    llvm::Type* getLLVMType(Type type);
};

class ExprAST {
public:
    Location loc;
    ExprAST(Location loc) : loc(std::move(loc)) {}
    virtual ~ExprAST() = default;
    virtual void dump(int indent = 0) const = 0;
    virtual llvm::Value* codegen(CodeGenContext& context) = 0;
    virtual std::string typeCheck() const = 0;
};

class NumberExprAST : public ExprAST {
    double val;
    Type type;
public:
    NumberExprAST(Location loc, double val, Type type) : ExprAST(std::move(loc)), val(val), type(type) {}
    void dump(int indent = 0) const override;
    llvm::Value* codegen(CodeGenContext& context) override;
    std::string typeCheck() const override { return ""; }
};

class VariableExprAST : public ExprAST {
    std::string name;
    Type type;
public:
    VariableExprAST(Location loc, const std::string &name, Type type) : ExprAST(std::move(loc)), name(name), type(type) {}
    void dump(int indent = 0) const override;
    llvm::Value* codegen(CodeGenContext& context) override;
    std::string typeCheck() const override { return ""; }
};

class BinaryExprAST : public ExprAST {
    char op;
    std::unique_ptr<ExprAST> lhs, rhs;
public:
    BinaryExprAST(Location loc, char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
        : ExprAST(loc), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
    void dump(int indent = 0) const override;
    llvm::Value* codegen(CodeGenContext& context) override;
    std::string typeCheck() const override;
};

class VarDeclExprAST : public ExprAST {
    std::string name;
    Type type;
    std::unique_ptr<ExprAST> initVal;
public:
    VarDeclExprAST(Location loc, const std::string &name, Type type, std::unique_ptr<ExprAST> initVal)
        : ExprAST(loc), name(name), type(type), initVal(std::move(initVal)) {}
    void dump(int indent = 0) const override;
    llvm::Value* codegen(CodeGenContext& context) override;
    std::string typeCheck() const override;
};

class PrintExprAST : public ExprAST {
    std::unique_ptr<ExprAST> arg;
public:
    PrintExprAST(Location loc, std::unique_ptr<ExprAST> arg) : ExprAST(std::move(loc)), arg(std::move(arg)) {}
    void dump(int indent = 0) const override;
    llvm::Value* codegen(CodeGenContext& context) override;
    std::string typeCheck() const override;
};

class ReturnExprAST : public ExprAST {
    std::unique_ptr<ExprAST> expr;
public:
    ReturnExprAST(Location loc, std::unique_ptr<ExprAST> expr) : ExprAST(std::move(loc)), expr(std::move(expr)) {}
    void dump(int indent = 0) const override;
    llvm::Value* codegen(CodeGenContext& context) override;
    std::string typeCheck() const override { return expr->typeCheck(); }
};

class FunctionAST {
    std::string name;
    std::vector<std::pair<std::string, Type>> args;
    Type returnType;
    std::vector<std::unique_ptr<ExprAST>> body;
public:
    FunctionAST(const std::string &name, std::vector<std::pair<std::string, Type>> args, Type returnType, std::vector<std::unique_ptr<ExprAST>> body)
        : name(name), args(std::move(args)), returnType(returnType), body(std::move(body)) {}
    void dump(int indent = 0) const;
    llvm::Function* codegen(CodeGenContext& context);
    std::string typeCheck() const;
};

class ModuleAST {
    std::vector<std::unique_ptr<FunctionAST>> functions;
public:
    void addFunction(std::unique_ptr<FunctionAST> function) {
        functions.push_back(std::move(function));
    }
    void dump(int indent = 0) const;
    void codegen(CodeGenContext& context);
    std::string typeCheck() const;
};
