%{
#include <stdio.h>

int yylex(void);
int yyerror(const char *msg);
%}

%token VARIABLE INVALID

%%

input:
    VARIABLE    { printf("Valid variable name\n"); }
  | INVALID     { printf("Invalid variable name\n"); }
  ;

%%

int main(void) {
    printf("Enter a variable name: ");
    yyparse();
    return 0;
}

int yyerror(const char *msg) {
    // we don’t really need error handling here
    return 0;
}
