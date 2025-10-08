#include <stdio.h>
#include <stdlib.h>

#define MAX 20

int transition[MAX][MAX];       // DFA transition table
int finalStates[MAX];           // 1 if final, 0 otherwise
int stateCount, inputCount, finalCount;
int distinguishable[MAX][MAX];  // Table for marking distinguishable pairs

void initializeTable() {
    for (int i = 0; i < stateCount; i++) {
        for (int j = 0; j < stateCount; j++) {
            distinguishable[i][j] = 0;
        }
    }
}

void markFinalVsNonFinal() {
    for (int i = 0; i < stateCount; i++) {
        for (int j = 0; j < stateCount; j++) {
            if ((finalStates[i] && !finalStates[j]) || (!finalStates[i] && finalStates[j])) {
                distinguishable[i][j] = 1;
                distinguishable[j][i] = 1;
            }
        }
    }
}

void markDistinguishablePairs() {
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < stateCount; i++) {
            for (int j = 0; j < i; j++) {
                if (distinguishable[i][j] == 0) {
                    for (int a = 0; a < inputCount; a++) {
                        int p = transition[i][a];
                        int q = transition[j][a];
                        if (distinguishable[p][q] || distinguishable[q][p]) {
                            distinguishable[i][j] = 1;
                            distinguishable[j][i] = 1;
                            changed = 1;
                            break;
                        }
                    }
                }
            }
        }
    } while (changed);
}

void printMinimizedDFA() {
    int group[MAX];
    for (int i = 0; i < stateCount; i++) group[i] = -1;

    int groupNum = 0;
    for (int i = 0; i < stateCount; i++) {
        if (group[i] == -1) {
            group[i] = groupNum;
            for (int j = i + 1; j < stateCount; j++) {
                if (distinguishable[i][j] == 0) {
                    group[j] = groupNum;
                }
            }
            groupNum++;
        }
    }

    printf("\nMinimized DFA has %d states:\n", groupNum);
    printf("Each group represents a new state:\n");
    for (int i = 0; i < groupNum; i++) {
        printf("Group %d: ", i);
        for (int j = 0; j < stateCount; j++) {
            if (group[j] == i) {
                printf("q%d ", j);
            }
        }
        printf("\n");
    }

    printf("\nMinimized DFA Transition Table:\n");
    printf("State\t");
    for (int a = 0; a < inputCount; a++) {
        printf("Input%d\t", a);
    }
    printf("\n");

    for (int i = 0; i < groupNum; i++) {
        printf("G%d\t", i);
        int repr = -1;
        for (int j = 0; j < stateCount; j++) {
            if (group[j] == i) {
                repr = j;
                break;
            }
        }
        for (int a = 0; a < inputCount; a++) {
            printf("G%d\t", group[transition[repr][a]]);
        }
        printf("\n");
    }
}

int main() {
    printf("Enter number of states: ");
    scanf("%d", &stateCount);

    printf("Enter number of input symbols: ");
    scanf("%d", &inputCount);

    printf("Enter the transition table (from state q0 to q%d):\n", stateCount - 1);
    for (int i = 0; i < stateCount; i++) {
        printf("From q%d:\n", i);
        for (int j = 0; j < inputCount; j++) {
            printf("  On input %d goes to: ", j);
            scanf("%d", &transition[i][j]);
        }
    }

    printf("Enter number of final states: ");
    scanf("%d", &finalCount);
    printf("Enter final states (e.g., 1 3 for q1 and q3):\n");
    for (int i = 0; i < stateCount; i++) finalStates[i] = 0;
    for (int i = 0; i < finalCount; i++) {
        int fs;
        scanf("%d", &fs);
        finalStates[fs] = 1;
    }

    initializeTable();
    markFinalVsNonFinal();
    markDistinguishablePairs();
    printMinimizedDFA();

    return 0;
}
