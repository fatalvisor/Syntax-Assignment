/* Originally based on "front.c". */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char keywords[10][10] = {"for", "if", "else", "while", "do", "int", "float", "switch"}; 
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
void statement();
void expr(void);
void term(void);
void factor(void);
void if_statement();
void for_loop();
void while_loop();
void error(void);

/* Character classes */
#define LETTER 0 
#define DIGIT 1 
#define SINGLEQUOTE 2
#define DOUBLEQUOTE 3
#define DECIMAL 4 
#define UNDERSCORE 5
#define UNKNOWN 99 

/* Token codes (Mostly from Homework 2) */
#define FLOAT_LIT 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 19
#define ADD_OP 20
#define SUB_OP 21
#define MULT_OP 22
#define DIV_OP 23
#define MOD_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define DECIMAL_POINT 27
#define LEFT_BRACK 28
#define SEMICOLON 29

/* Token codes (Mostly from Homework 3) */
#define FOR_CODE 30
#define IF_CODE 31
#define ELSE_CODE 32
#define WHILE_CODE 33
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37

/******************************************************/
/* main driver */
int main(void) {
	/* QUESTION 1 TEST: Tests the new keyword tokens added. */
	if ((in_fp = fopen("front.in", "r")) == NULL) {
	  printf("ERROR - cannot open front.in \n");
  }
	else {
	 	getChar();
    do {
      lex();
    } while (nextToken != EOF);
	}

  printf("=========================================\n");

  /* QUESTION 2 TEST: Tests mathematical operations. */
  // NOTE: Uncomment to test.
  /*if ((in_fp = fopen("front.in", "r")) == NULL) {
	  printf("ERROR - cannot open front.in \n");
  }
  else {
    getChar();
    lex();  
    expr();
  }*/
  
  printf("=========================================\n");

  /* QUESTION 3 TEST: Tests "if", "for", and "while" statements. */
  // NOTE: Uncomment to test.
  /*if ((in_fp = fopen("front.in", "r")) == NULL) {
	  printf("ERROR - cannot open front.in \n");
  }
  else {
    getChar();
    lex();  

    // eBNF: if '(' <expression> ')' <statement> [else <statement>]
    if_statement();

    // eBNF: <for_loop> --> for '(' <expression> ';' <expression> ';' <expression> ')' <statement>
    //for_loop();

    // eBNF: while '(' <expression> ')' <statement>
    //while_loop();
  }*/
	return 0;
}

/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
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

    case '%':
      addChar();
      nextToken = MOD_OP;
      break;

    case '=':
      addChar();
      nextToken = ASSIGN_OP;
      break;

    case '.':
      addChar();
      nextToken = DECIMAL_POINT;
      break;

    case ';':
      addChar();
      nextToken = SEMICOLON;
      break;

    default:
      addChar();
      nextToken = EOF;
      break;
  }
  return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} 
  else
	  printf("Error - lexeme is too long \n");
}

/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
  nextChar = getc(in_fp);
  if (nextChar != EOF) {
    if (isalpha(nextChar)) {
      charClass = LETTER;
    }
    else if (isdigit(nextChar)) {
      charClass = DIGIT;
    }
    else if (nextChar == '_') {
      charClass = UNDERSCORE;
    }
    else {
      charClass = UNKNOWN;
    }
  } 
  else
    charClass = EOF;
}

/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	  getChar();
}

/******************************************************/
/* lex - A simple lexical analyzer */
int lex(void) {
  lexLen = 0;
  getNonBlank();
  switch (charClass) {
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE) {
        addChar();
        getChar();
      }

      // FOR_CODE:
      if (strcmp(lexeme, keywords[0]) == 0) {
        nextToken = FOR_CODE;
        break;
      }

      // IF_CODE:
      else if (strcmp(lexeme, keywords[1]) == 0) {
        nextToken = IF_CODE;
        break;
      }

      // ELSE_CODE:
      else if (strcmp(lexeme, keywords[2]) == 0) {
        nextToken = ELSE_CODE;
        break;
      }

      // WHILE_CODE:
      else if (strcmp(lexeme, keywords[3]) == 0) {
        nextToken = WHILE_CODE;
        break;
      }

      // DO_CODE:
      else if (strcmp(lexeme, keywords[4]) == 0) {
        nextToken = DO_CODE;
        break;
      }

      // INT_CODE:
      else if (strcmp(lexeme, keywords[5]) == 0) {
        nextToken = INT_CODE;
        break;
      }

      // FLOAT_CODE:
      else if (strcmp(lexeme, keywords[6]) == 0) {
        nextToken = FLOAT_CODE;
        break;
      }

      // SWITCH_CODE:
      else if (strcmp(lexeme, keywords[7]) == 0) {
        nextToken = SWITCH_CODE;
        break;
      }

      // User-Defined Identifiers
      else {
        nextToken = IDENT;
        break;
      }
      
    case UNDERSCORE:
      // User-Defined Identifiers
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE) {
        addChar();
        getChar();
      }
      nextToken = IDENT;
      break;

    /* Integer & Float literals */
    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
        addChar();
        getChar();
      }

      if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
        addChar();
        getChar();
        nextToken = FLOAT_LIT;
        break;
      }

      if(nextChar == 'E' || nextChar == 'e') {
        addChar();
        getChar();
        if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
          addChar();
          getChar();
          nextToken = FLOAT_LIT;
          break;
        }
        if(nextChar == '-'){
          addChar();
          getChar();
          while(charClass == DIGIT){
            addChar();
            getChar();
            if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
              addChar();
              getChar();
              nextToken = FLOAT_LIT;
              break;
            }
          }
          nextToken = FLOAT_LIT;
          break;
        } 
        else if(charClass == DIGIT){
          while(charClass == DIGIT){
            addChar();
            getChar();
            if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
              addChar();
              getChar();
              nextToken = FLOAT_LIT;
              break;
            }
          }
          nextToken = FLOAT_LIT;
          break;
        }
      }
      
      if (charClass == UNKNOWN && nextChar == '.'){
        addChar();
        getChar();
        while (charClass == DIGIT) {
          addChar();
          getChar();
          if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
            addChar();
            getChar();
            nextToken = FLOAT_LIT;
            break;
          }
          if(nextChar == 'E' || nextChar == 'e'){
            addChar();
            getChar();
            if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
              addChar();
              getChar();
              nextToken = FLOAT_LIT;
              break;
            }
            if(nextChar == '-'){
              addChar();
              getChar();
              while(charClass == DIGIT){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
              }
              nextToken = FLOAT_LIT;
              break;
            } 
            else if(charClass == DIGIT){
              while(charClass == DIGIT){
                addChar();
                getChar();
                if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                  addChar();
                  getChar();
                  nextToken = FLOAT_LIT;
                  break;
                }
              }
              nextToken = FLOAT_LIT;
              break;
            }
          }
        }
        nextToken = FLOAT_LIT;
      } 
      else {
        nextToken = INT_LIT;
      }
      break;
      
    case UNKNOWN:
      if (nextChar == '.'){
        addChar();
        getChar();
        if(charClass == DIGIT){
          addChar();
          getChar();
          while(charClass == DIGIT){
            addChar();
            getChar();
            if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
              addChar();
              getChar();
              nextToken = FLOAT_LIT;
              break;
            }
            else if(nextChar == 'E' || nextChar == 'e'){
              addChar();
              getChar();
              if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                addChar();
                getChar();
                nextToken = FLOAT_LIT;
                break;
              }
              if(nextChar == '-'){
                addChar();
                getChar();
                while(charClass == DIGIT){
                  addChar();
                  getChar();
                  if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                    addChar();
                    getChar();
                    nextToken = FLOAT_LIT;
                    break;
                  }
                }
                nextToken = FLOAT_LIT;
                break;
              } 
              else if(charClass == DIGIT){
                while(charClass == DIGIT){
                  addChar();
                  getChar();
                  if (nextChar == 'f' || nextChar == 'F' || nextChar == 'l' || nextChar == 'L'){
                    addChar();
                    getChar();
                    nextToken = FLOAT_LIT;
                    break;
                  }
                }
                nextToken = FLOAT_LIT;
                break;
              }
            }
            nextToken = FLOAT_LIT;
          }
        } 
        else {
          nextToken = DECIMAL_POINT;
        } 
      } 
      else {
        lookup(nextChar);
        getChar();
      }
      break;
  
    /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
        break;
  } 
  /* End of switch */
  printf("Next token is: %d, Next lexeme is %s\n",
  nextToken, lexeme);
  return nextToken;
  /* End of function lex */
}

/******************************************************/
/* Identifies the type of statement being used. */
void statement() {
  switch (nextToken) {
    case IF_CODE:
      if_statement();
      break;
    case FOR_CODE:
      for_loop();
      break;
    case WHILE_CODE:
      while_loop();
      break;
    default:
      error();
  }
}

/* Made up of terms, which themselves are made of factors. */
void expr(void) {
	printf("Enter <expr>\n");
	/* Parse the first term */
	term();
	/* As long as the next token is + or -, get
	the next token and parse the next term. */
  while (nextToken == ADD_OP || nextToken == SUB_OP) {
    lex();
    term();
  }
  printf("Exit <expr>\n");
}

/* term
Parses strings in the language generated by the rule:
<term> -> <factor> {(* | / | % ) <factor>}
*/
void term(void) {
	printf("Enter <term>\n");
	/* Parse the first factor. */
	factor();
	/* As long as the next token is *, /, or %, get the
	 next token and parse the next factor. */
  while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
    lex();
    factor();
  }
  printf("Exit <term>\n");
}

/* factor
Parses strings in the language generated by the rule:
<factor> -> id | int_constant | ( <expr> )
*/
void factor(void) {
	printf("Enter <factor>\n");
	/* Determine which RHS. */
  if (nextToken == IDENT || nextToken == INT_LIT) {
    /* Get the next token. */
    lex();
  }
  /* If the RHS is ( <expr> ), call lex to pass over the
    left parenthesis, call expr, and check for the right
    parenthesis. */
  else if (nextToken == LEFT_PAREN) {
    lex();
    expr();
    if (nextToken == RIGHT_PAREN) {
      lex();
    }
    else {
      error();
    }
  }
  /* It was not an id, an integer literal, or a left
    parenthesis. */
  else {
    error();
  }
  printf("Exit <factor>\n");
}

/* Syntax of if-else statements. */
void if_statement() {
  if (nextToken != IF_CODE) {
    error();
  }
  else {
    lex();
    if (nextToken != LEFT_PAREN) {
      error();
    }
    else {
      lex();
      expr();
      if (nextToken != RIGHT_PAREN) {
        error();
      }
      else {
        lex();
        statement();
        if (nextToken == ELSE_CODE) {
          lex();
          statement();
        }
      }
    }
  }
}

/* Syntax of while-loop statements. */
void while_loop() {
  if (nextToken != WHILE_CODE) {
    error();
  }
  else {
    lex();
    if (nextToken != LEFT_PAREN) {
      error();
    }
    else {
      lex();
      expr();
      if (nextToken != RIGHT_PAREN) {
        error();
      }
      else {
        lex();
        statement();
        if (nextToken == ELSE_CODE) {
          lex();
          statement();
        }
      }
    }
  }
}

/* Syntax of for-loop statements. */
void for_loop() {
  if (nextToken != FOR_CODE) {
    error();
  }
  else {
    lex();
    if (nextToken != LEFT_PAREN) {
      error();
    }
    else {
      lex();
      expr();
      if (nextToken != SEMICOLON) {
        error();
      }
      else {
        lex();
        expr();
        if (nextToken != SEMICOLON) {
          error();
        }
        else {
          lex();
          expr();
          if (nextToken != RIGHT_PAREN) {
            error();
          }
          else {
            lex();
            statement();
          }
        }
      }
    }
  }
}

void error(void) {
 exit(1);
}
