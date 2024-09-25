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
    IF,
    ELSE,
    FOR,
    RETURN,
    WHILE,
    SWITCH,
    CASE,
    DEFAULT,
    CONTINUE,
    BREAK,
    GOTO,
    TYPEDEF,
    DATA_TYPE,
    STRING,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    AND,
    OR,
    NOT,
    EQ,
    NEQ,
    LT,
    GT,
    LE,
    GE,
    INC,
    DEC,
    ARROW,
    SCOPE,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COMMA
};
%}

%%

"fn"                   { yylval = strdup(yytext); return FN; }
"if"                   { yylval = strdup(yytext); return IF; }
"else"                 { yylval = strdup(yytext); return ELSE; }
"for"                  { yylval = strdup(yytext); return FOR; }
"return"               { yylval = strdup(yytext); return RETURN; }
"while"                { yylval = strdup(yytext); return WHILE; }
"switch"               { yylval = strdup(yytext); return SWITCH; }
"case"                 { yylval = strdup(yytext); return CASE; }
"default"              { yylval = strdup(yytext); return DEFAULT; }
"continue"             { yylval = strdup(yytext); return CONTINUE; }
"break"                { yylval = strdup(yytext); return BREAK; }
"goto"                 { yylval = strdup(yytext); return GOTO; }
"typedef"              { yylval = strdup(yytext); return TYPEDEF; }
"u8"|"u16"|"u32"|"u64"|"i8"|"i16"|"i32"|"i64"|"float"|"double" | "string" { yylval = strdup(yytext); return DATA_TYPE; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval = strdup(yytext); return IDENTIFIER; }

[0-9]+                 { yylval = strdup(yytext); return INTEGER; }
[0-9]*"."[0-9]+        { yylval = strdup(yytext); return FLOAT; }
\"([^"\\]|\\[ntvbrfa\\'\"?])*\"  { yylval = strdup(yytext); return STRING; }


"="                    { yylval = strdup(yytext); return ASSIGN; }
"+"                    { yylval = strdup(yytext); return PLUS; }
"-"                    { yylval = strdup(yytext); return MINUS; }
"*"                    { yylval = strdup(yytext); return STAR; }
"/"                    { yylval = strdup(yytext); return SLASH; }
"&&"                   { yylval = strdup(yytext); return AND; }
"||"                   { yylval = strdup(yytext); return OR; }
"!"                    { yylval = strdup(yytext); return NOT; }
"=="                   { yylval = strdup(yytext); return EQ; }
"!="                   { yylval = strdup(yytext); return NEQ; }
"<"                    { yylval = strdup(yytext); return LT; }
">"                    { yylval = strdup(yytext); return GT; }
"<="                   { yylval = strdup(yytext); return LE; }
">="                   { yylval = strdup(yytext); return GE; }
"++"                   { yylval = strdup(yytext); return INC; }
"--"                   { yylval = strdup(yytext); return DEC; }
"->"                   { yylval = strdup(yytext); return ARROW; }
"::"                   { yylval = strdup(yytext); return SCOPE; }

"("                    { yylval = strdup(yytext); return LPAREN; }
")"                    { yylval = strdup(yytext); return RPAREN; }
"{"                    { yylval = strdup(yytext); return LBRACE; }
"}"                    { yylval = strdup(yytext); return RBRACE; }
";"                    { yylval = strdup(yytext); return SEMICOLON; }
","                    { yylval = strdup(yytext); return COMMA; }

[ \t\n\r]+             { /* Ignore whitespace */ }
"//".*                 { /* Ignore comments */ }

.                      { cerr << "Unknown character: " << yytext << endl; exit(1); }

%%

