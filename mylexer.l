%{
#include <stdio.h>
#include <string.h>
%}

/* 定义关键字的 token */
%{
#define KEYWORD_ELSE 1
#define KEYWORD_IF 2
#define KEYWORD_INT 3
#define KEYWORD_RETURN 4
#define KEYWORD_VOID 5
#define KEYWORD_WHILE 6
%}

/* 定义符号的 token */
%{
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
%}

%{
#define ID 26
#define NUM 27
%}

%x COMMENT

%%

else    { printf("Keyword: else\n"); return KEYWORD_ELSE; }
if      { printf("Keyword: if\n"); return KEYWORD_IF; }
int     { printf("Keyword: int\n"); return KEYWORD_INT; }
return  { printf("Keyword: return\n"); return KEYWORD_RETURN; }
void    { printf("Keyword: void\n"); return KEYWORD_VOID; }
while   { printf("Keyword: while\n"); return KEYWORD_WHILE; }

[+]     { return PLUS; }
[-]     { return MINUS; }
[*]     { return TIMES; }
[/]     { return DIVIDE; }
[<]     { return LT; }
[<][=]  { return LEQ; }
[>]     { return GT; }
[>][=]  { return GEQ; }
[=][=]  { return EQ; }
[!][=]  { return NEQ; }
[=]     { return ASSIGN; }
[;]     { return SEMICOLON; }
[,]     { return COMMA; }
[(]     { return LPAREN; }
[)]     { return RPAREN; }
[{]     { return LBRACE; }
[}]     { return RBRACE; }
[\[]    { return LBRACKET; }
[\]]    { return RBRACKET; }

[a-zA-Z][a-zA-Z0-9]*   { printf("ID: %s\n", yytext); return ID; }
[0-9]+                  { printf("NUM: %s\n", yytext); return NUM; }

[ \t\n]                 ;

"/*"                    { BEGIN(COMMENT); }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>[^\n]*        ;

%%

int yywrap() {
    return 1;
}
