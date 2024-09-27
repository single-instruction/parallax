%option c++
/* %option yyclass="Lexer" */

%{
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

int yylex(void);

// To store the token's value
char* yylval = NULL;

// Token types
enum yytokentype {
    FN = 1,
    // IF,
    // ELSE,
    // FOR,
    RETURN,
    TYPE,
    STRING,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    ASSIGN,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    PRINT, // SHOULD BE CHANGED TO FUNCTION LATER
};
// TODO: Free strdup or use a LLVM construct (LLVM::StringRef ?) to safely manage the memory of yytext strings.
%}

%%

"fn"                   { yylval = strdup(yytext); return FN; }
"return"               { yylval = strdup(yytext); return RETURN; }
"print"                { return PRINT; }

"i8"|"i16"|"i32"|"i64"  { yylval = strdup(yytext); return TYPE; }
"u8"|"u16"|"u32"|"u64"  { yylval = strdup(yytext); return TYPE; }
"f32"|"f64"             { yylval = strdup(yytext); return TYPE; }
"string"                { yylval = strdup(yytext); return TYPE; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval = strdup(yytext); return IDENTIFIER; }

[0-9]+                 { yylval = strdup(yytext); return INTEGER; }
[0-9]*"."[0-9]+        { yylval = strdup(yytext); return FLOAT; }
\"([^"\\]|\\[ntvbrfa\\'\"?])*\"  { yylval = strdup(yytext); return STRING; }


"="                    { yylval = strdup(yytext); return ASSIGN; }
"+"                    { yylval = strdup(yytext); return PLUS; }
"-"                    { yylval = strdup(yytext); return MINUS; }
"*"                    { yylval = strdup(yytext); return MUL; }
"/"                    { yylval = strdup(yytext); return DIV; }

"("                    { yylval = strdup(yytext); return LPAREN; }
")"                    { yylval = strdup(yytext); return RPAREN; }
"{"                    { yylval = strdup(yytext); return LBRACE; }
"}"                    { yylval = strdup(yytext); return RBRACE; }
";"                    { yylval = strdup(yytext); return SEMICOLON; }

[ \t\n\r]+             { /* Ignore whitespace */ }
"//".*                 { /* Ignore comments */ }

.                      { cerr << "Unknown character: " << yytext << endl; exit(1); }

%%

