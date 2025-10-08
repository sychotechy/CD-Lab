#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 20

char productions[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
int numProductions;
void findFirst(char *, int, int);
void findFollow(char c);
int isTerminal(char c);
int productionIndex(char c);
int visitedFollow[MAX] = {0};
int main()
{
    int i;
    char ch;
    printf("Enter number of productions: ");
    scanf("%d", &numProductions);
    printf("Enter productions (e.g., S->AB):\n");

    for (i = 0; i < numProductions; i++)
    {
        scanf("%s", productions[i]);
    }
    for (i = 0; i < numProductions; i++)
    {
        char symbol = productions[i][0];
        findFirst(first[i], i, 0);
    }
    printf("\nFIRST Sets:\n");
    for (i = 0; i < numProductions; i++)
    {
        printf("FIRST(%c) = { ", productions[i][0]);
        for (int j = 0; first[i][j] != '\0'; j++)
        {
            printf("%c ", first[i][j]);
        }
        printf("}\n");
    }
    for (i = 0; i < numProductions; i++)
    {
        if (i == 0)
        {
            follow[i][0] = '$'; // Start symbol gets $
        }
        findFollow(productions[i][0]);
    }
    printf("\nFOLLOW Sets:\n");
    for (i = 0; i < numProductions; i++)
    {
        printf("FOLLOW(%c) = { ", productions[i][0]);
        for (int j = 0; follow[i][j] != '\0'; j++)
        {
            printf("%c ", follow[i][j]);
        }
        printf("}\n");
    }
    return 0;
}
void addToSet(char *set, char val)
{
    for (int i = 0; set[i] != '\0'; i++)
    {
        if (set[i] == val)
            return;
    }
    int len = strlen(set);
    set[len] = val;
    set[len + 1] = '\0';
}
int isTerminal(char c)
{
    return !(c >= 'A' && c <= 'Z');
}
int productionIndex(char c)
{
    for (int i = 0; i < numProductions; i++)
    {
        if (productions[i][0] == c)
            return i;
    }
    return -1;
}
void findFirst(char *result, int prodIndex, int pos)
{
    char symbol = productions[prodIndex][pos + 3];
    if (symbol == '\0')
        return;
    if (isTerminal(symbol))
    {
        addToSet(result, symbol);
    }
    else
    {
        for (int i = 0; i < numProductions; i++)
        {
            if (productions[i][0] == symbol)
            {
                findFirst(result, i, 0);
            }
        }
    }
}
void findFollow(char c)
{
    int i, j;
    int idx = productionIndex(c);
    if (idx == -1)
        return;
    for (i = 0; i < numProductions; i++)
    {
        for (j = 3; productions[i][j] != '\0'; j++)
        {
            if (productions[i][j] == c)
            {
                if (productions[i][j + 1] != '\0')
                {
                    char next = productions[i][j + 1];
                    if (isTerminal(next))
                    {
                        addToSet(follow[idx], next);
                    }
                    else
                    {
                        int ni = productionIndex(next);
                        for (int k = 0; first[ni][k] != '\0'; k++)
                        {
                            if (first[ni][k] != '#')
                            {
                                addToSet(follow[idx], first[ni][k]);
                            }
                        }
                    }
                }
                else if (productions[i][0] != c)
                {
                    findFollow(productions[i][0]);

                    int pi = productionIndex(productions[i][0]);
                    for (int k = 0; follow[pi][k] != '\0'; k++)
                    {
                        addToSet(follow[idx], follow[pi][k]);
                    }
                }
            }
        }
    }
}