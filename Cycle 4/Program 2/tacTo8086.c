#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINES 1000
#define MAXLEN 256
#define MAXSYM 1000

char *lines[MAXLINES];
int nlines = 0;

char *syms[MAXSYM];
int symCount = 0;

void trim(char *s)
{
    char *p = s;
    while (*p && isspace((unsigned char)*p))
        p++;
    if (p != s)
        memmove(s, p, strlen(p) + 1);
    int L = strlen(s);
    while (L > 0 && isspace((unsigned char)s[L - 1]))
        s[--L] = '\0';
}

int is_number(const char *s)
{
    if (!s || !*s)
        return 0;
    const char *p = s;
    if (*p == '+' || *p == '-')
        p++;
    while (*p)
    {
        if (!isdigit((unsigned char)*p))
            return 0;
        p++;
    }
    return 1;
}

int find_sym(const char *s)
{
    for (int i = 0; i < symCount; ++i)
        if (strcmp(syms[i], s) == 0)
            return i;
    return -1;
}
void add_sym(const char *s)
{
    if (!s || !*s)
        return;
    if (is_number(s))
        return;
    if (find_sym(s) == -1)
    {
        syms[symCount++] = strdup(s);
    }
}

const char *relop_to_jmp(const char *rel)
{
    if (strcmp(rel, "<") == 0)
        return "JL";
    if (strcmp(rel, ">") == 0)
        return "JG";
    if (strcmp(rel, "<=") == 0)
        return "JLE";
    if (strcmp(rel, ">=") == 0)
        return "JGE";
    if (strcmp(rel, "==") == 0)
        return "JE";
    if (strcmp(rel, "!=") == 0)
        return "JNE";
    return NULL;
}

int main()
{
    char buf[MAXLEN];
    printf("Enter Three Address Code (TAC) lines (Ctrl+D to finish):\n");
    while (fgets(buf, sizeof(buf), stdin))
    {
        char tmp[MAXLEN];
        strcpy(tmp, buf);
        trim(tmp);
        if (tmp[0] == '\0')
            continue;
        if (nlines < MAXLINES)
            lines[nlines++] = strdup(tmp);
    }
    if (nlines == 0)
    {
        fprintf(stderr, "No TAC input provided (stdin empty). Provide TAC via stdin or a file redirect.\n");
        return 1;
    }
    for (int i = 0; i < nlines; ++i)
    {
        char line[MAXLEN];
        strcpy(line, lines[i]);
        trim(line);
        int L = strlen(line);
       
        if (line[L - 1] == ':')
            continue;
        
        if (strncmp(line, "goto ", 5) == 0)
            continue;
        
        if (strncmp(line, "if ", 3) == 0)
        {
            
            char a[MAXLEN], rel[8], b[MAXLEN], tmpw[8], lbl[MAXLEN];
            int got = sscanf(line, "if %s %s %s %s %s", a, rel, b, tmpw, lbl);
            if (got >= 3)
            {
                add_sym(a);
                add_sym(b);
            }
            continue;
        }
        char *eq = strstr(line, "=");
        if (eq)
        {
            char lhs[MAXLEN];
            strncpy(lhs, line, eq - line);
            lhs[eq - line] = '\0';
            trim(lhs);
            add_sym(lhs);
            char rhs[MAXLEN];
            strcpy(rhs, eq + 1);
            trim(rhs);
            char tok[MAXLEN];
            int tpos = 0;
            for (int k = 0; rhs[k]; ++k)
            {
                if (isalnum((unsigned char)rhs[k]) || rhs[k] == '_')
                    tok[tpos++] = rhs[k];
                else
                {
                    if (tpos > 0)
                    {
                        tok[tpos] = '\0';
                        add_sym(tok);
                        tpos = 0;
                    }
                }
            }
            if (tpos > 0)
            {
                tok[tpos] = '\0';
                add_sym(tok);
            }
        }
    }

    printf("; Generated 8086 Assembly (MASM/TASM style)\n");
    printf(".MODEL small\n");
    printf(".STACK 100h\n");
    printf(".DATA\n");
    for (int i = 0; i < symCount; ++i)
    {
        printf("    %s DW 0\n", syms[i]);
    }
    printf(".CODE\n");
    printf("main PROC\n");
    printf("    MOV AX,@DATA\n");
    printf("    MOV DS,AX\n\n");

   
    for (int i = 0; i < nlines; ++i)
    {
        char line[MAXLEN];
        strcpy(line, lines[i]);
        trim(line);
        int L = strlen(line);
        if (L == 0)
            continue;

        /* label */
        if (line[L - 1] == ':')
        {
            printf("%s\n", line);
            continue;
        }


        if (strncmp(line, "goto ", 5) == 0)
        {
            char lbl[MAXLEN];
            sscanf(line + 5, "%s", lbl);
            printf("    JMP %s\n", lbl);
            continue;
        }


        if (strncmp(line, "if ", 3) == 0)
        {
            char a[MAXLEN], rel[8], b[MAXLEN], tmpw[8], lbl[MAXLEN];
            int got = sscanf(line, "if %s %s %s %s %s", a, rel, b, tmpw, lbl);
            if (got >= 3)
            {
               
                if (is_number(a))
                    printf("    MOV AX, %s\n", a);
                else
                    printf("    MOV AX, %s\n", a);
                if (is_number(b))
                    printf("    CMP AX, %s\n", b);
                else
                    printf("    CMP AX, %s\n", b);
                const char *jmp = relop_to_jmp(rel);
                if (jmp && got >= 5)
                    printf("    %s %s\n", jmp, lbl);
                else if (jmp && got >= 4)
                    printf("    %s %s\n", jmp, tmpw);
                else
                    printf("    ; unknown conditional or missing label in: %s\n", line);
            }
            else
            {
                printf("    ; malformed if: %s\n", line);
            }
            continue;
        }

        
        char *eq = strstr(line, "=");
        if (!eq)
        {
            printf("    ; unrecognized TAC: %s\n", line);
            continue;
        }
        char lhs[MAXLEN];
        strncpy(lhs, line, eq - line);
        lhs[eq - line] = '\0';
        trim(lhs);
        char rhs[MAXLEN];
        strcpy(rhs, eq + 1);
        trim(rhs);

        char op = 0;
        int split_at = -1;
        for (int k = 0; rhs[k]; ++k)
        {
            if (rhs[k] == '+' || rhs[k] == '-' || rhs[k] == '*' || rhs[k] == '/')
            {
                op = rhs[k];
                split_at = k;
                break;
            }
        }

        if (op == 0)
        {
            
            if (is_number(rhs))
            {
                printf("    MOV AX, %s\n", rhs);
            }
            else
            {
                printf("    MOV AX, %s\n", rhs);
            }
            printf("    MOV %s, AX\n", lhs);
            continue;
        }

       
        char left[MAXLEN], right[MAXLEN];
        strncpy(left, rhs, split_at);
        left[split_at] = '\0';
        trim(left);
        strcpy(right, rhs + split_at + 1);
        trim(right);

        if (is_number(left))
            printf("    MOV AX, %s\n", left);
        else
            printf("    MOV AX, %s\n", left);

        if (op == '+')
        {
            if (is_number(right))
                printf("    ADD AX, %s\n", right);
            else
                printf("    ADD AX, %s\n", right);
        }
        else if (op == '-')
        {
            if (is_number(right))
                printf("    SUB AX, %s\n", right);
            else
                printf("    SUB AX, %s\n", right);
        }
        else if (op == '*')
        {
            if (is_number(right))
                printf("    MOV BX, %s\n", right);
            else
                printf("    MOV BX, %s\n", right);
            printf("    IMUL BX\n");
        }
        else if (op == '/')
        {
            if (is_number(right))
                printf("    MOV BX, %s\n", right);
            else
                printf("    MOV BX, %s\n", right);
            printf("    CWD\n");
            printf("    IDIV BX\n");
        }
        else
        {
            printf("    ; unsupported op in: %s\n", line);
        }

        printf("    MOV %s, AX\n", lhs);
    }


    printf("\n    MOV AH,4Ch\n");
    printf("    INT 21h\n");
    printf("main ENDP\nEND main\n");

    return 0;
}