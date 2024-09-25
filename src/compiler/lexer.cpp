#include <iostream>
#include <cstdlib>
// Include the generated lexer code
#include "lex.yy.c"

// Define yywrap function
extern "C" int yywrap() {
    return 1;  // Return 1 to indicate end of input
}

bool has_px_extension(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) {
        return false;
    }
    std::string extension = filename.substr(pos + 1);
    return extension == "px";
}

// Function to get the token name based on the token type
const char* getTokenName(int tokenType) {
    // Token name mapping code remains unchanged
    switch (tokenType) {
        case FN: return "FN";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case FOR: return "FOR";
        case RETURN: return "RETURN";
        case WHILE: return "WHILE";
        case SWITCH: return "SWITCH";
        case CASE: return "CASE";
        case DEFAULT: return "DEFAULT";
        case CONTINUE: return "CONTINUE";
        case BREAK: return "BREAK";
        case GOTO: return "GOTO";
        case TYPEDEF: return "TYPEDEF";
        case DATA_TYPE: return "DATA TYPE";
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case FLOAT: return "FLOAT";
        case ASSIGN: return "ASSIGN";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case STAR: return "STAR";
        case SLASH: return "SLASH";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case EQ: return "EQ";
        case NEQ: return "NEQ";
        case LT: return "LT";
        case GT: return "GT";
        case LE: return "LE";
        case GE: return "GE";
        case INC: return "INC";
        case DEC: return "DEC";
        case ARROW: return "ARROW";
        case SCOPE: return "SCOPE";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        case STRING: return "STRING";
        default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Check if the file has .px extension
    if (!has_px_extension(argv[1])) {
        std::cerr << "Error: The file must have .px extension." << std::endl;
        return 1;
    }

    // Open the input file
    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    yyin = input_file;
    int token;
    while ((token = yylex()) != 0) {
        std::cout << "{Token Name: " << getTokenName(token)
                  << ", Token Type: " << token
                  << ", Value: " << yylval << "}" << std::endl;
        free(yylval);  
    }

    fclose(input_file);
    return 0;
}
