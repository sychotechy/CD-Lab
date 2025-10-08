%{
#include "for_syntax.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%

for|FOR             { return FOR; }
int|float|char      { return TYPE; }
[ \t\n]+            { /* ignore whitespace */ }
[0-9]+              { yylval.num = atoi(yytext); return NUM; }
"=="|"<="|">="|"!="|"<"|">"  { return RELOP; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.id = strdup(yytext); return ID; }
"="                 { return '='; }
"++"                { return INC; }
"+="                { return PL_EQ; }
"-="                { return MI_EQ; }
"+"                 { return '+'; }
"-"                 { return '-'; }
";"                 { return ';'; }
"("                 { return '('; }
")"                 { return ')'; }
.                   { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}
