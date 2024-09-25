#include <iostream>
#include <cstdlib>

// Declare the yylex function
extern "C" int yylex();

// Declare yyin if you want to read from a file
extern "C" FILE* yyin;

// Define yywrap function
extern "C" int yywrap() {
    return 1;  // Return 1 to indicate end of input
}

int main() {
    // If you want to read from a file, uncomment the following lines
    // FILE* input_file = fopen("input.txt", "r");
    // if (!input_file) {
    //     std::cerr << "Could not open input file" << std::endl;
    //     return 1;
    // }
    // yyin = input_file;

    int token;
    while ((token = yylex()) != 0) {
        std::cout << "Token: " << token << std::endl;
    }

    // If you opened a file, close it
    // fclose(input_file);

    return 0;
}

// Include the generated lexer code
#include "lex.yy.c"