/*Grammar Documentation:

Throughout the development of this Lexical Analyzer/Parser program, the 
following grammatical rules have been utilized.

<expr> ::= <term> {(+|-) <term>}
<term> ::= <factor> {(*|/) <factor>}
<factor> ::= <id> | <int> | ( <expr> )
<id> ::= <letter> {<letter>|<digit>}
<int> ::= <digit> {<digit>}

With these rules, the program abides by the BNF grammar. Throughout the code, inline comments
will be made documenting the use of such rules, as well as highlighting the use of proper naming convention in the C programming language.  

*/

#include <stdio.h>
#include <ctype.h>

/* Global declarations */

// Variables 

//Variable declaration makes use of correct naming conventions for variables in C. 
char lexeme [100];
int nextToken;
FILE *in_fp, *out_fp, *fopen();
int charClass;
int lexLen;
int token;
char nextChar;
int currentIndentation = 0;

// Functions

//Similar to variables, camel case is used to abide by naming conventions for functions in C.
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
void expr();
void term();
void factor();

// Char Classes

//Here, capitilzation is used in accordance with proper naming conventions on named constants.
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
        // Open the output file for writing
        if ((out_fp = fopen("output.txt", "w")) == NULL) {
            printf("ERROR - cannot open output.txt for writing \n");
            return 1;
        }
        
        getChar(); 
        do {
            lex();
            expr();
        } while (nextToken != EOF);
        
    }
}

/* Lexical Analyzer */

// lookup - a function to lookup operators and parentheses and return the token 
//lookup uses the following rules from predefined grammar: <factor> ::= <id> | <int> | ( <expr> ), <id> ::= <letter> {<letter>|<digit>}
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

// addChar - a function to add to lexeme
//addChar utilizes the following rule from predefined grammar: <expr> ::= <term> {(+|-) <term>}, <int> ::= <digit> {<digit>}
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

//getChar - function to get char from input file and set charClass
//getChar uses the following rule form the predefined grammar: <expr> ::= <term> {(+|-) <term>}
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

// getNonBlank - a function to call getChar until it returns a non-whitespace character
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}


// lex - a simple lexical analyzer for arithmetic expressions 
//Uses the following rules from BNF grammar: <factor> ::= <id> | <int> | ( <expr> ), <id> ::= <letter> {<letter>|<digit>}, <int> ::= <digit> {<digit>}
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

void error() {
    printf("Error in parsing\n");
}

/***********************************/

/* Recursive Descent Parser */

//expr() uses following grammatical rule: <expr> ::= <term> {(+|-) <term>}
void expr() {
    fprintf(out_fp, "%*s[expr\n", currentIndentation, "");    currentIndentation += 4;
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        fprintf(out_fp, "%*s[%c]\n", currentIndentation, "", lexeme[0]);
        lex();
        term();
    }
    currentIndentation -= 4;
    fprintf(out_fp, "%*s]\n", currentIndentation, "");
}

//term() uses following grammatical rule: <term> ::= <factor> {(*|/) <factor>}
void term() {
    fprintf(out_fp, "%*s[term\n", currentIndentation, "");
    currentIndentation += 4;
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        fprintf(out_fp, "%*s[%c]\n", currentIndentation, "", lexeme[0]);
        lex();
        factor();
    }
    currentIndentation -= 4;
    fprintf(out_fp, "%*s]\n", currentIndentation, "");
}
//factor() uses following grammatical rule: <factor> ::= <id> | <int> | ( <expr> )
void factor() {
    fprintf(out_fp, "%*s[factor\n", currentIndentation, "");
    currentIndentation += 4;
    if (nextToken == IDENT || nextToken == INT_LIT) {
        fprintf(out_fp, "%*s[id [%s]]\n", currentIndentation, "", lexeme);
        lex();
    } else if (nextToken == LEFT_PAREN) {
        fprintf(out_fp, "%*s[LPAREN]\n", currentIndentation, "");
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            fprintf(out_fp, "%*s[RPAREN]\n", currentIndentation, "");
            lex();
        } else {
            error(); // Error handling for missing closing parenthesis
        }
    } else {
        error(); // Error handling for unexpected token
    }
    currentIndentation -= 4;
    fprintf(out_fp, "%*s]\n", currentIndentation, "");
}
