/* Parser.c */
#include "LexicalAnalyzer.h"
#include <stdio.h>
#include <ctype.h>

/* Add global variables for token and lexeme */
extern int nextToken;
extern char lexeme[];


/* Function declarations */
void expr();
void term();
void factor();
void getNextToken(); // Declare a function to get the next token

/* main driver */
int main() {
    /* Open the input data file and process its contents */
    FILE *in_fp;
    printf("line 21");

    if ((in_fp = fopen("front.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in\n");
        return 1; // Return with error status
    }
    printf("line 27");
    /* Initialize global variables for the lexical analyzer */
    charClass = lexLen = token = nextToken = 0;
    nextChar = ' ';

    printf("line 32");
    /* Start processing the input */
    getChar(); // Initialize nextChar
    do {
        lex(); // Perform lexical analysis
        expr(); // Perform parsing
    } while (nextToken != EOF);
    printf("print 39");

    /* Close the input file */
    fclose(in_fp);

    return 0; // Return with success status
}

/* Function to get the next token from the lexical analyzer */
void getNextToken() {
    nextToken = lex(); // Assuming lex() returns the next token
}

/* Parser functions */

void expr() {
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        getNextToken(); // Get the next token from the lexical analyzer
        term();
    }
}

void term() {
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        getNextToken(); // Get the next token from the lexical analyzer
        factor();
    }
}

void factor() {
    if (nextToken == IDENT || nextToken == INT_LIT) {
        getNextToken(); // Get the next token from the lexical analyzer
    } else if (nextToken == LEFT_PAREN) {
        getNextToken(); // Get the next token from the lexical analyzer
        expr();
        if (nextToken == RIGHT_PAREN) {
            getNextToken(); // Get the next token from the lexical analyzer
        } else {
            // Handle error: ')' expected
        }
    } else {
        // Handle error: Unexpected token
    }
}


