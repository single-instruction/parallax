%require "3.2"

%{
#include "Lexer.h"
#include "AST.h"
#include <iostream>
#include <vector>
#include <memory>

extern int yylex();
void yyerror(const char* s);

std::unique_ptr<ModuleAST> moduleAST;
%}

%union {
    char* identifier;
    char* type;
    int intValue;
    double floatValue;
    char* stringValue;
    ExprAST* expr;
    std::vector<std::unique_ptr<ExprAST>>* exprList;
    FunctionAST* function;
    std::vector<std::pair<std::string, Type>>* paramList;
}

%token <identifier> IDENTIFIER
%token <type> TYPE
%token <intValue> INT_LITERAL
%token <floatValue> FLOAT_LITERAL
%token <stringValue> STRING_LITERAL
%token FUNCTION RETURN PRINT

%type <expr> expr primary_expr
%type <exprList> expr_list
%type <function> function
%type <paramList> param_list

%%

module  : function { moduleAST = std::make_unique<ModuleAST>(); moduleAST->addFunction(std::unique_ptr<FunctionAST>($1)); }
        | module function { moduleAST->addFunction(std::unique_ptr<FunctionAST>($2)); }
        ;

function: FUNCTION IDENTIFIER '(' param_list ')' TYPE '{' expr_list '}'
        {
            $$ = new FunctionAST($2, *$4, stringToType($6), std::move(*$8));
            delete $4;
            delete $8;
        }
        ;

param_list: /* empty */ { $$ = new std::vector<std::pair<std::string, Type>>(); }
          | IDENTIFIER TYPE { $$ = new std::vector<std::pair<std::string, Type>>(); $$->emplace_back($1, stringToType($2)); }
          | param_list ',' IDENTIFIER TYPE { $1->emplace_back($3, stringToType($4)); $$ = $1; }
          ;

expr_list: expr ';' { $$ = new std::vector<std::unique_ptr<ExprAST>>(); $$->push_back(std::unique_ptr<ExprAST>($1)); }
         | expr_list expr ';' { $1->push_back(std::unique_ptr<ExprAST>($2)); $$ = $1; }
         ;

expr: primary_expr { $$ = $1; }
    | expr '+' expr { $$ = new BinaryExprAST({}, '+', std::unique_ptr<ExprAST>($1), std::unique_ptr<ExprAST>($3)); }
    | expr '-' expr { $$ = new BinaryExprAST({}, '-', std::unique_ptr<ExprAST>($1), std::unique_ptr<ExprAST>($3)); }
    | expr '*' expr { $$ = new BinaryExprAST({}, '*', std::unique_ptr<ExprAST>($1), std::unique_ptr<ExprAST>($3)); }
    | expr '/' expr { $$ = new BinaryExprAST({}, '/', std::unique_ptr<ExprAST>($1), std::unique_ptr<ExprAST>($3)); }
    | IDENTIFIER '=' expr { $$ = new VarDeclExprAST({}, $1, Type::F64, std::unique_ptr<ExprAST>($3)); } // Assuming F64 for now
    | PRINT '(' expr ')' { $$ = new PrintExprAST({}, std::unique_ptr<ExprAST>($3)); }
    | RETURN expr { $$ = new ReturnExprAST({}, std::unique_ptr<ExprAST>($2)); }
    ;

primary_expr: INT_LITERAL { $$ = new NumberExprAST({}, $1, Type::I32); }
            | FLOAT_LITERAL { $$ = new NumberExprAST({}, $1, Type::F64); }
            | STRING_LITERAL { $$ = new VariableExprAST({}, $1, Type::String); }
            | IDENTIFIER { $$ = new VariableExprAST({}, $1, Type::F64); } // Assuming F64 for now
            | '(' expr ')' { $$ = $2; }
            ;

%%

void yyerror(const char* s) {
    std::cerr << "Parse error: " << s << std::endl;
}

Type stringToType(const std::string& typeStr) {
    if (typeStr == "i8") return Type::I8;
    if (typeStr == "i16") return Type::I16;
    if (typeStr == "i32") return Type::I32;
    if (typeStr == "i64") return Type::I64;
    if (typeStr == "u8") return Type::U8;
    if (typeStr == "u16") return Type::U16;
    if (typeStr == "u32") return Type::U32;
    if (typeStr == "u64") return Type::U64;
    if (typeStr == "f32") return Type::F32;
    if (typeStr == "f64") return Type::F64;
    if (typeStr == "string") return Type::String;
    return Type::Void; // Default or error case
}
