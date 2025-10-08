%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
int yyerror(const char *s);
%}

%union {
    int num;
    char *id;
}

%token FOR TYPE
%token <id> ID
%token <num> NUM
%token RELOP INC PL_EQ MI_EQ

%%

stmt:
    FOR '(' assign ';' condition ';' increment ')' ';'
        { printf("Valid for loop syntax\n"); }
    ;

assign:
      TYPE ID '=' NUM
    | ID '=' NUM
    ;

condition:
      ID RELOP NUM
    ;

increment:
      ID '=' ID '+' NUM
    | ID INC
    | ID PL_EQ NUM
    | ID MI_EQ NUM
    ;

%%

int yyerror(const char *s) {
    printf("Syntax Error\n");
    return 0;
}

int main() {
    printf("Enter a 'for' loop statement:\n");
    yyparse();
    return 0;
}
