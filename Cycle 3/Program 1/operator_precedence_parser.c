#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define SIZE 100
char prec_table[7][7] = {
    {'>', '>', '<', '<', '<', '<', '>'}, // +
    {'>', '>', '<', '<', '<', '<', '>'}, // -
    {'>', '>', '>', '>', '<', '<', '>'}, // *
    {'>', '>', '>', '>', '<', '<', '>'}, // /
    {'>', '>', '>', '>', '>', '<', '>'}, // ^
    {'>', '>', '>', '>', '>', ' ', '>'}, // i
    {'<', '<', '<', '<', '<', '<', 'A'}  // $
};
int get_index(char symbol)
{
    switch (symbol)
    {
    case '+':
        return 0;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 3;
    case '^':
        return 4;
    case 'i':
        return 5;
    case '$':
        return 6;
    case 'E':
        return 5;
    default:
        return -1;
    }
}
char top_terminal(char stack[], int top)
{
    for (int i = top; i >= 0; i--)
    {
        if (!isupper(stack[i]))
            return stack[i];
    }
    return '$';
}
void print_step(char stack[], int top, char input[], int ptr, const char *action)
{
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);
    printf("\t\t");
    for (int i = ptr; i < strlen(input); i++)
        printf("%c", input[i]);
    printf("\t\t%s\n", action);
}
int main()
{
    char stack[SIZE], input[SIZE];
    int top = 0, ptr = 0;
    printf("Enter expression (use i for operand, end with $): ");
    scanf("%s", input);
    stack[0] = '$';

    printf("\n%-20s %-20s %-10s\n", "STACK", "INPUT", "ACTION");
    while (1)
    {
        char a = top_terminal(stack, top);
        char b = input[ptr];

        int ai = get_index(a), bi = get_index(b);

        if (ai == -1 || bi == -1)
        {
            printf("Invalid symbol encountered!\n");
            break;
        }
        char relation = prec_table[ai][bi];
        if (relation == '<' || relation == '=')
        {
            stack[++top] = b;
            ptr++;
            print_step(stack, top, input, ptr, "Shift");
        }
        else if (relation == '>')
        {
            if (stack[top] == 'i')
            {
                stack[top] = 'E';
            }
            else if (top >= 2 && stack[top] == 'E' &&
                     (stack[top - 1] == '+' || stack[top - 1] == '-' ||
                      stack[top - 1] == '*' || stack[top - 1] == '/' ||
                      stack[top - 1] == '^') &&
                     stack[top - 2] == 'E')
            {
                top -= 2;
                stack[top] = 'E';
            }
            print_step(stack, top, input, ptr, "Reduce");
        }
        else if (relation == 'A')
        {
            print_step(stack, top, input, ptr, "Accept");
            break;
        }
        else
        {
            print_step(stack, top, input, ptr, "Error");
            break;
        }
    }
    return 0;
}
