%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%token INT VOID ID NUM
%token IF ELSE WHILE RETURN
%token LEQ LT GT GEQ EQ NEQ
%token PLUS MINUS TIMES DIVIDE
%token ASSIGN SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET

%union {
    char *string;
    int num;
}

%type <string> program declaration_list declaration var_declaration type_specifier fun_declaration params param_list compound_stmt local_declarations statement_list statement expression_stmt selection_stmt iteration_stmt return_stmt expression var simple_expression additive_expression term factor call args arg_list

%%

program: declaration_list
        ;

declaration_list: declaration_list declaration
                | declaration
                ;

declaration: var_declaration
           | fun_declaration
           ;

var_declaration: type_specifier ID SEMICOLON
               | type_specifier ID LBRACKET NUM RBRACKET SEMICOLON
               ;

type_specifier: INT
              | VOID
              ;

fun_declaration: type_specifier ID LPAREN params RPAREN compound_stmt
                ;

params: param_list
      | VOID
      ;

param_list: param_list COMMA param
          | param
          ;

param: type_specifier ID
     | type_specifier ID LBRACKET RBRACKET
     ;

compound_stmt: LBRACE local_declarations statement_list RBRACE
              ;

local_declarations: local_declarations var_declaration
                  | /* empty */
                  ;

statement_list: statement_list statement
              | /* empty */
              ;

statement: expression_stmt
         | compound_stmt
         | selection_stmt
         | iteration_stmt
         | return_stmt
         ;

expression_stmt: expression SEMICOLON
               | SEMICOLON
               ;

selection_stmt: IF LPAREN expression RPAREN statement
              | IF LPAREN expression RPAREN statement ELSE statement
              ;

iteration_stmt: WHILE LPAREN expression RPAREN statement
              ;

return_stmt: RETURN SEMICOLON
           | RETURN expression SEMICOLON
           ;

expression: var ASSIGN expression
          | simple_expression
          ;

var: ID
   | ID LBRACKET expression RBRACKET
   ;

simple_expression: additive_expression relop additive_expression
                 | additive_expression
                 ;

relop: LEQ
     | LT
     | GT
     | GEQ
     | EQ
     | NEQ
     ;

additive_expression: additive_expression PLUS term
                    | additive_expression MINUS term
                    | term
                    ;

term: term TIMES factor
    | term DIVIDE factor
    | factor
    ;

factor: LPAREN expression RPAREN
      | var
      | call
      | NUM
      ;

call: ID LPAREN args RPAREN
    ;

args: arg_list
    | /* empty */
    ;

arg_list: arg_list COMMA expression
        | expression
        ;

%%

int main() {
    printf("Enter C- program:\n");
    yyparse();
    printf("Parsing completed.\n");
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    exit(1);
}
