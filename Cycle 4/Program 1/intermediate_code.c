#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100
typedef struct
{
    char items[MAX][10];
    int top;
} Stack;
void push(Stack *s, const char *item)
{
    strcpy(s->items[++(s->top)], item);
}
char *pop(Stack *s)
{
    return s->items[(s->top)--];
}
char *peek(Stack *s)
{
    return s->items[s->top];
}
int isEmpty(Stack *s)
{
    return s->top == -1;
}
int precedence(char op)
{
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}
int isOperator(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}
int tempVarCount = 1;
void generateTAC(char postfix[])
{
    Stack s;
    s.top = -1;
    char operand1[10], operand2[10], result[10], temp[10];
    int i = 0;
    while (postfix[i])
    {
        if (isalpha(postfix[i]))
        {
            char op[2] = {postfix[i], '\0'};
            push(&s, op);
        }
        else if (isOperator(postfix[i]))
        {
            strcpy(operand2, pop(&s));
            strcpy(operand1, pop(&s));
            sprintf(temp, "t%d", tempVarCount++);
            printf("%s = %s %c %s\n", temp, operand1, postfix[i], operand2);
            push(&s, temp);
        }
        i++;
    }
}
void infixToPostfix(char infix[], char postfix[])
{
    Stack s;
    s.top = -1;
    int i = 0, j = 0;
    while (infix[i])
    {
        if (isalpha(infix[i]))
        {
            postfix[j++] = infix[i];
        }
        else if (infix[i] == '(')
        {
            char temp[2] = {infix[i], '\0'};
            push(&s, temp);
        }
        else if (infix[i] == ')')
        {
            while (!isEmpty(&s) && peek(&s)[0] != '(')
            {
                postfix[j++] = pop(&s)[0];
            }
            pop(&s); // Remove '('
        }
        else if (isOperator(infix[i]))
        {
            while (!isEmpty(&s) && precedence(peek(&s)[0]) >= precedence(infix[i]))
            {
                postfix[j++] = pop(&s)[0];
            }
            char temp[2] = {infix[i], '\0'};
            push(&s, temp);
        }
        i++;
    }
    while (!isEmpty(&s))
    {
        postfix[j++] = pop(&s)[0];
    }
    postfix[j] = '\0';
}
int main()
{
    char input[100];
    char infix[100];
    char lhs;
    char postfix[100];
    printf("Enter expression (e.g., a = b + c * d):\n");
    fgets(input, sizeof(input), stdin);
    int i = 0, j = 0;
    while (input[i])
    {
        if (!isspace(input[i]))
        {
            infix[j++] = input[i];
        }
        i++;
    }
    infix[j] = '\0';
    lhs = infix[0];
    if (infix[1] != '=')
    {
        printf("Invalid expression.\n");
        return 1;
    }
    char rhs[100];
    strcpy(rhs, infix + 2); // Skip a=
    infixToPostfix(rhs, postfix);
    printf("\nIntermediate Code (Three Address Code):\n");
    generateTAC(postfix);
    printf("%c = t%d\n", lhs, tempVarCount - 1);
    return 0;
}
