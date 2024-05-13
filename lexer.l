%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ELSE 1
#define IF 2
#define INT 3
#define RETURN 4
#define VOID 5
#define WHILE 6
#define PLUS 7
#define MINUS 8
#define TIMES 9
#define DIVIDE 10
#define LT 11
#define LEQ 12
#define GT 13
#define GEQ 14
#define EQ 15
#define NEQ 16
#define ASSIGN 17
#define SEMICOLON 18
#define COMMA 19
#define LPAREN 20
#define RPAREN 21
#define LBRACKET 22
#define RBRACKET 23
#define LBRACE 24
#define RBRACE 25
#define ID 26
#define NUM 27

int yylval;
%}


letter [a-zA-Z]
digit [0-9]
id {letter}({letter}|{digit})*
num {digit}+
%%

[ \t\n]  ;

"/*"([^*]|"*"+[^*/])*"*"+"/"  ;

else    { printf("ELSE\n"); return ELSE; }
if      { printf("IF\n"); return IF; }
int     { printf("INT\n"); return INT; }
return  { printf("RETURN\n"); return RETURN; }
void    { printf("VOID\n"); return VOID; }
while   { printf("WHILE\n"); return WHILE; }
{id}    { yylval = strdup(yytext); printf("ID(%s)\n", yytext); return ID; }
{num}   { sscanf(yytext, "%d", &yylval); printf("NUM(%s)\n", yytext); return NUM; }
"+"     { printf("PLUS\n"); return PLUS; }
"-"     { printf("MINUS\n"); return MINUS; }
"*"     { printf("TIMES\n"); return TIMES; }
"/"     { printf("DIVIDE\n"); return DIVIDE; }
"<"     { printf("LT\n"); return LT; }
"<="    { printf("LEQ\n"); return LEQ; }
">"     { printf("GT\n"); return GT; }
">="    { printf("GEQ\n"); return GEQ; }
"=="    { printf("EQ\n"); return EQ; }
"!="    { printf("NEQ\n"); return NEQ; }
"="     { printf("ASSIGN\n"); return ASSIGN; }
";"     { printf("SEMICOLON\n"); return SEMICOLON; }
","     { printf("COMMA\n"); return COMMA; }
"("     { printf("LPAREN\n"); return LPAREN; }
")"     { printf("RPAREN\n"); return RPAREN; }
"["     { printf("LBRACKET\n"); return LBRACKET; }
"]"     { printf("RBRACKET\n"); return RBRACKET; }
"{"     { printf("LBRACE\n"); return LBRACE; }
"}"     { printf("RBRACE\n"); return RBRACE; }
%%
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error opening input file %s\n", argv[1]);
        return 1;
    }

    yyin = input_file;

    while (yylex() != 0) {}

    fclose(input_file);
    return 0;
}
