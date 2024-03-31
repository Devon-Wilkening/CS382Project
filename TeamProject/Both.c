// Both.c
#include <stdio.h>
#include <ctype.h>

/* Global declarations */

/* Variables */

char lexeme [100];
int nextToken;
FILE *in_fp, *fopen();
int charClass;
int lexLen;
int token;
char nextChar;

/* Function declarations */

void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
void expr();
void term();
void factor();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int main() {
    if ((in_fp = fopen("front.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } else {
        getChar();
        expr(); 
        do {
            lex();
        } while (nextToken != EOF);
    }
}




/* lookup - a function to lookup operators and parentheses
and return the token */

int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}
/*****************************************************/

/* addChar - a function to add  to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

/*****************************************************/

/* getChar - a function to get the next character of
input and determine its character class 
void getChar() {
    if ((nextChar = getc(in_fp)) == EOF) {
        printf("line 111");
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
            else charClass = UNKNOWN;
    }
    else
        printf("line 119");
        charClass = EOF;
}
*/

void getChar() {
    nextChar = getc(in_fp);
    if (nextChar != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}
/******************************************************/

/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
    /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
    /* Parse integer literals */
            case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
    /* Parentheses and operators */
            case UNKNOWN:
                lookup(nextChar);
                getChar();
                break;
    /* EOF */
            case EOF:
                nextToken = EOF;
                lexeme[0] = 'E';
                lexeme[1] = 'O';
                lexeme[2] = 'F';
                lexeme[3] = 0;
                break;
        } /* End of switch */
    printf("Next token is: %d, Next lexeme is %s\n",
        nextToken, lexeme);
    return nextToken;
}

void expr() {
    printf("[expr\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("]\n");
} 


void term() {
    printf("[term\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("]\n");
} 

void factor() {
    printf("[factor\n");
    if (nextToken == IDENT || nextToken == INT_LIT)
        lex();
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
                lex();
            else
                printf("Error");
        } 
    else printf("Error");
    } 
    printf("]\n");;
} 