#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

char states[MAX][3], inputs[MAX][3], transitions[MAX][3][3], eclosure[MAX][MAX][3];
int stateCount, inputCount, transCount = 0;

void computeEclosure(FILE *fp, char *state, int index, int *ecCount) {
    char s1[3], inp[3], s2[3];
    int i;
    int visited[MAX] = {0};
    int front = 0, rear = 0;
    strcpy(eclosure[index][(*ecCount)++], state);
    visited[index] = 1;

    while (front < *ecCount) {
        strcpy(s1, eclosure[index][front++]);
        rewind(fp);
        while (fscanf(fp, "%s %s %s", s1, inp, s2) != EOF) {
            if (strcmp(s1, eclosure[index][front - 1]) == 0 && strcmp(inp, "e") == 0) {
                int alreadyPresent = 0;
                for (i = 0; i < *ecCount; i++) {
                    if (strcmp(eclosure[index][i], s2) == 0) {
                        alreadyPresent = 1;
                        break;
                    }
                }
                if (!alreadyPresent) {
                    strcpy(eclosure[index][(*ecCount)++], s2);
                }
            }
        }
    }
}

void removeEpsilonNFA(FILE *fp) {
    int i, j, k, ecCount;
    char s1[3], inp[3], s2[3];

    printf("\nNew transitions (NFA without epsilon):\n");

    for (i = 0; i < stateCount; i++) {
        ecCount = 0;
        computeEclosure(fp, states[i], i, &ecCount);

        for (j = 0; j < ecCount; j++) {
            rewind(fp);
            while (fscanf(fp, "%s %s %s", s1, inp, s2) != EOF) {
                if (strcmp(inp, "e") != 0 && strcmp(s1, eclosure[i][j]) == 0) {
                    printf("%s -- %s --> %s\n", states[i], inp, s2);
                }
            }
        }
    }
}

int main() {
    int i;
    FILE *fp = fopen("enfa.txt", "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Enter number of states: ");
    scanf("%d", &stateCount);
    printf("Enter state names (e.g., q0 q1 ...):\n");
    for (i = 0; i < stateCount; i++) {
        scanf("%s", states[i]);
    }

    printf("\nConverting NFA with ε-transitions to NFA without ε-transitions...\n");
    removeEpsilonNFA(fp);

    fclose(fp);
    return 0;
}
