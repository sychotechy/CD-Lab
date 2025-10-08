%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyerror(const char *msg);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%left UMINUS

%%

input:
      input line
    | /* empty */
    ;

line:
      '\n'
    | expr '\n'     { printf("= %d\n", $1); }
    ;

expr:
      NUMBER                  { $$ = $1; }
    | expr '+' expr           { $$ = $1 + $3; }
    | expr '-' expr           { $$ = $1 - $3; }
    | expr '*' expr           { $$ = $1 * $3; }
    | expr '/' expr           {
                                 if ($3 == 0) {
                                     printf("Error: Division by zero\n");
                                     $$ = 0;
                                 } else {
                                     $$ = $1 / $3;
                                 }
                               }
    | '-' expr %prec UMINUS   { $$ = -$2; }
    | '(' expr ')'            { $$ = $2; }
    ;

%%

int yyerror(const char *msg) {
    printf("Syntax Error: %s\n", msg);
    return 0;
}

int main(void) {
    printf("Enter expressions (press Ctrl+D to exit):\n");
    yyparse();
    return 0;
}
