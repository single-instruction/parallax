%{
#include <iostream>
#include <cstdlib>






using namespace std;

int yylex(void);
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
    TYPE,
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


"fn"                   { return FN; }
"if"                   { return IF; }
"else"                 { return ELSE; }
"for"                  { return FOR; }
"return"               { return RETURN; }
"while"                 { return WHILE; }
"switch"                { return SWITCH; }
"case"                  { return CASE; }
"default"               { return DEFAULT; }
"continue"              { return CONTINUE; }
"break"                 { return BREAK; }
"goto"                  { return GOTO; }
"typedef"               { return TYPEDEF; }
"u8"|"u16"|"u32"|"u64"|"i8"|"i16"|"i32"|"i64"|"float"|"double" { return TYPE; }


[a-zA-Z_][a-zA-Z0-9_]* {  return IDENTIFIER; }


[0-9]+                 {  return INTEGER; }
[0-9]*"."[0-9]+        { return FLOAT; }


"="                    { return ASSIGN; }
"+"                    { return PLUS; }
"-"                    { return MINUS; }
"*"                    { return STAR; }
"/"                    { return SLASH; }
"&&"                   { return AND; }
"||"                   { return OR; }
"!"                    { return NOT; }
"=="                   { return EQ; }
"!="                   { return NEQ; }
"<"                    { return LT; }
">"                    { return GT; }
"<="                   { return LE; }
">="                   { return GE; }
"++"                   { return INC; }
"--"                   { return DEC; }
"->"                   { return ARROW; }
"::"                   { return SCOPE; }


"("                    { return LPAREN; }
")"                    { return RPAREN; }
"{"                    { return LBRACE; }
"}"                    { return RBRACE; }
";"                    { return SEMICOLON; }
","                    { return COMMA; }

[ \t\n\r]+             { /* Ignore whitespace */ }
"//".*                 { /* Ignore comments */ }


.                      { cerr << "Unknown character: " << yytext << endl; exit(1); }

%%

