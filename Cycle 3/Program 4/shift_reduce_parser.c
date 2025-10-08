#include <stdio.h>
#include <string.h>
#define MAX 100
char stack[MAX];
int top = -1;
char input[MAX];
int ip = 0;   // input pointer
void push(char c) {
    stack[++top] = c;
}
void pop(int n) {
    top -= n;
}
void print_row(const char *action) {
    char stack_str[MAX] = {0};
    for (int j = 0; j <= top; j++) {
        int len = strlen(stack_str);
        stack_str[len] = stack[j];
        stack_str[len + 1] = '\0';
    }
    char input_str[MAX] = {0};
    strncpy(input_str, &input[ip], MAX - 1);
    printf("| %-14s | %-14s | %-26s |\n", stack_str, input_str, action);
}
int isOperator(char c) {
    return c == '+' || c == '*';
}
int precedence(char c) {
    if (c == '+') return 1;
    if (c == '*') return 2;
    return 0;
}
int reduce(int next_prec) {
    if (top >= 0 && stack[top] == 'i') {
        stack[top] = 'F';
        print_row("Reduce by F -> i");
        return 1;
    }
    if (top >= 2 && stack[top-2] == '(' && stack[top-1] == 'E' && stack[top] == ')') {
        pop(3);
        push('F');
        print_row("Reduce by F -> (E)");
        return 1;
    }
    if (top >= 2 && stack[top-2] == 'T' && stack[top-1] == '*' && stack[top] == 'F' && next_prec <= 2) {
        pop(3);
        push('T');
        print_row("Reduce by T -> T * F");
        return 1;
    }
    if (top >= 2 && stack[top-2] == 'E' && stack[top-1] == '+' && stack[top] == 'T' && next_prec <= 1) {
        pop(3);
        push('E');
        print_row("Reduce by E -> E + T");
        return 1;
    }
    if (top >= 0 && stack[top] == 'F' && next_prec <= 2) {
        stack[top] = 'T';
        print_row("Reduce by T -> F");
        return 1;
    }
    if (top >= 0 && stack[top] == 'T' && next_prec <= 1) {
        stack[top] = 'E';
        print_row("Reduce by E -> T");
        return 1;
    }
    return 0;
}
int main() {
    printf("Enter input string (use i for id, end with $): ");
    scanf("%s", input);
    int len = strlen(input);
    if (input[len - 1] != '$') {
        printf("Error: Input must end with '$'\n");
        return 1;
    }
    push('$');
    printf("+----------------+----------------+----------------------------+\n");
    printf("|     Stack      |     Input      |        Action Taken        |\n");
    printf("+----------------+----------------+----------------------------+\n");
    print_row("Initial");
    while (ip < len) {
        char curr = input[ip];
        if (curr == '$') {
            ip++;
            continue;
        }
        if (isOperator(curr)) {
            int curr_prec = precedence(curr);
            int j = top;
            char topOp = 0;
            while (j >= 0) {
                if (isOperator(stack[j])) {
                    topOp = stack[j];
                    break;
                }
                j--;
            }
            while (topOp && precedence(topOp) >= curr_prec) {
                 
if (!reduce(curr_prec)) break;
                j = top;
                topOp = 0;
                while (j >= 0) {
                    if (isOperator(stack[j])) {
                        topOp = stack[j];
                        break;
                    }
                    j--;
                }}}
        push(curr);
        ip++;
        print_row("Shift");
        int next_prec = (ip < len) ? (isOperator(input[ip]) ? precedence(input[ip]) : 0) : 0;
	while (reduce(next_prec)) {
        }}
    int end_prec = 0;
    while (reduce(end_prec)) {}
    if (top == 1 && stack[0] == '$' && stack[1] == 'E') {
        print_row("Input is accepted");
    } else {
        print_row("Input is rejected");
    }
    printf("+----------------+----------------+----------------------------+\n");
    return 0;
} 
