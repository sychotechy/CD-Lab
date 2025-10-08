%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex();

typedef struct node {
    struct node *left, *right;
    char val[50];   // Increased buffer size
    int label;
} NODE;

NODE* makeNode(char *val, NODE *left, NODE *right) {
    NODE *temp = (NODE *) malloc(sizeof(NODE));
    strncpy(temp->val, val, sizeof(temp->val)-1);
    temp->val[sizeof(temp->val)-1] = '\0';
    temp->left = left;
    temp->right = right;
    temp->label = 0;
    return temp;
}

NODE* synTree;
%}

%union {
    char *str;
    struct node *node;
}

%token <str> PL MI MUL DIV OP CL POW ID VAL
%type <node> s e t f g

%left PL MI
%left MUL DIV
%right POW

%%

s : e { synTree = $1; }
  ;

e : e PL t { $$ = makeNode($2, $1, $3); free($2); }
  | e MI t { $$ = makeNode($2, $1, $3); free($2); }
  | t      { $$ = $1; }
  ;

t : t MUL f { $$ = makeNode($2, $1, $3); free($2); }
  | t DIV f { $$ = makeNode($2, $1, $3); free($2); }
  | f       { $$ = $1; }
  ;

f : g POW f { $$ = makeNode($2, $1, $3); free($2); }
  | g       { $$ = $1; }
  ;

g : OP e CL       { $$ = $2; }
  | ID            { $$ = makeNode($1, NULL, NULL); free($1); }
  | VAL           { $$ = makeNode($1, NULL, NULL); free($1); }
  ;

%%

void inOrder(NODE* root) {
    if (root) {
        inOrder(root->left);
        printf("%s ", root->val);
        inOrder(root->right);
    }
}

void preOrder(NODE* root) {
    if (root) {
        printf("%s ", root->val);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void postOrder(NODE* root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%s ", root->val);
    }
}

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    printf("Enter expression:\n");
    yyparse();

    printf("\nIn Order:\n");
    inOrder(synTree);
    printf("\nPre Order:\n");
    preOrder(synTree);
    printf("\nPost Order:\n");
    postOrder(synTree);
    printf("\n");

    return 0;
}
