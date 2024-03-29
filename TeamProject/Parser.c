/* Starting point from chatgpt*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/* Global variables */
int nextToken;
char lexeme[100];

/* Function to get the next token from lexical analyzer */
void lex() {
    // Code to get the next token from lexical analyzer
}

/* Error handler function */
void error() {
    fprintf(stderr, "Syntax error\n");
    exit(EXIT_FAILURE);
}

/* Function prototypes */
void expr();
void term();
void factor();

/* Function for <term> nonterminal */
void term() {
    printf("Enter <term>\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("Exit <term>\n");
}

/* Function for <factor> nonterminal */
void factor() {
    printf("Enter <factor>\n");
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex();
    } else if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        } else {
            error();
        }
    } else {
        error();
    }
    printf("Exit <factor>\n");
}

/* Function for <expr> nonterminal */
void expr() {
    printf("Enter <expr>\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("Exit <expr>\n");
}

/* Main function */
int main() {
    // Initialize parser
    // Get the first token
    lex();
    
    // Start parsing
    expr();  // Start with <expr>
    
    // Check for end of input
    if (nextToken != -1) {
        error();
    }
    
    printf("Parsing successful.\n");
    
    return 0;
}
