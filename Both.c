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

    // Opens front.in for reading
    if ((in_fp = fopen("front.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } else {

        // Open the output file for writing
        if ((out_fp = fopen("output.txt", "w")) == NULL) {
            printf("ERROR - cannot open output.txt for writing \n");
            return 1;
        }

        // Reads first character from input file 
        getChar(); 

        do {
            // Lexical Analysis 
            lex();
            // Parses
            expr();

        // End of input file has been reached 
        } while (nextToken != EOF);
        
    }
}

/* Lexical Analyzer */

// lookup - a function to lookup operators and parentheses and return the token 
//lookup uses the following rules from predefined grammar: <factor> ::= <id> | <int> | ( <expr> ), <id> ::= <letter> {<letter>|<digit>}
int lookup(char ch) {
    switch (ch) {
        // Gives '(' value LEFT_PAREN
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        // Gives ')' value RIGHT_PAREN
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        // Gives '+' value ADD_OP
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        // Gives '-' value SUB_OP
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        // Gives '*' value MULT_OP
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        // Gives '/' value DIV_OP
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        // Character doesn't fit anything. Must be end of file. 
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

    // Make sure lexeme has room 
    if (lexLen <= 98) {

        // Adds next character to lexeme array
        lexeme[lexLen++] = nextChar;
        // Adds null terminator to end of lexeme array to mark the end of array
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

//getChar - function to get char from input file and set charClass
//getChar uses the following rule form the predefined grammar: <expr> ::= <term> {(+|-) <term>}
void getChar() {

    // Reads next character in file and assigns to variable
    nextChar = getc(in_fp);

    if (nextChar != EOF) {
        // If nextChar is letter, belongs to LETTER charClass
        if (isalpha(nextChar))
            charClass = LETTER;
        // If nextChar is number, belongs to DIGIT charClass
        else if (isdigit(nextChar))
            charClass = DIGIT;
        // If nextChar is something else, belongs to UNKNOWN charClass
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

    // Keeps track of lexeme length
    lexLen = 0;
    // Make sure white space is not selected 
    getNonBlank();

    // Finds current char's class 
    switch (charClass) {

        // If char is a letter ...
        case LETTER:
            // Adds current char to lexeme 
            addChar();
            // Reads next char 
            getChar();
            // Continues adding chars to lexeme if theyre nums or letters
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            // Shows identifier token has been found
            nextToken = IDENT;
            break;
        
        // If char is a number ... 
        case DIGIT:
            // Adds current char to lexeme 
            addChar();
            // Reads next char 
            getChar();
            // Continues adding chars to lexeme if theyre nums
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            // Shows integer literal token has been found
            nextToken = INT_LIT;
            break;

        // If char is something else ...
        case UNKNOWN:
            // Determine token type of unknown character
            lookup(nextChar);
            // Reads next char
            getChar();
            break;
    
        // If there is no char ...
        case EOF:
            // Set nextToken to EOF
            nextToken = EOF;
            // Contstruct lexeme "EOF"
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
        } 

    // Print current char token type and lexeme
    printf("Next token is: %d, Next lexeme is %s\n",nextToken, lexeme);
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
