%{
#include <stdio.h>
#include <string.h>
char block_substring[5];
int contain_block_substring(const char *str)
{
    return strstr(str, block_substring) != NULL;
}
%}

%%

.*\n    {
            yytext[yyleng - 1] = '\0';   // remove newline
            if (contain_block_substring(yytext))
            {
                printf("rejected: contains %s\n", block_substring);
            }
            else
            {
                printf("accepted: %s\n", yytext);
            }
        }

%%

int main()
{
    char name[100];
    printf("enter the name: ");
    fgets(name, sizeof(name), stdin);

    name[strcspn(name, "\n")] = '\0';

    // take first 4 characters as block substring
    strncpy(block_substring, name, 4);
    block_substring[4] = '\0';

    printf("block substring: %s\n", block_substring);
    printf("enter the string:\n");

    yylex();
    return 0;
}
