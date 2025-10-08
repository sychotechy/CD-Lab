#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

char states[MAX][3], inputs[MAX][3];
char transitionTable[MAX][MAX][3];
int stateCount = 0, inputCount = 0, transCount = 0;

char dfaStates[MAX][MAX * 3];
int dfaTrans[MAX][MAX];
int dfaStateCount = 0;

int isPresent(char list[MAX][3], int count, char *item) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], item) == 0) return 1;
    }
    return 0;
}

int isDFAStatePresent(char *state) {
    for (int i = 0; i < dfaStateCount; i++) {
        if (strcmp(dfaStates[i], state) == 0) return i;
    }
    return -1;
}

void addTransition(char *from, char *input, char *to) {
    strcpy(transitionTable[transCount][0], from);
    strcpy(transitionTable[transCount][1], input);
    strcpy(transitionTable[transCount][2], to);
    transCount++;
}

void getNextState(char *currentSet, char *inputSymbol, char *result) {
    char parts[MAX][3];
    int partCount = 0;
    char *token = strtok(currentSet, ",");
    while (token != NULL) {
        strcpy(parts[partCount++], token);
        token = strtok(NULL, ",");
    }

    result[0] = '\0';
    for (int i = 0; i < partCount; i++) {
        for (int j = 0; j < transCount; j++) {
            if (strcmp(transitionTable[j][0], parts[i]) == 0 &&
                strcmp(transitionTable[j][1], inputSymbol) == 0) {
                if (!strstr(result, transitionTable[j][2])) {
                    strcat(result, transitionTable[j][2]);
                    strcat(result, ",");
                }
            }
        }
    }

    int len = strlen(result);
    if (len > 0 && result[len - 1] == ',') result[len - 1] = '\0';
}

void convertNFAToDFA() {
    strcpy(dfaStates[dfaStateCount++], "q0");

    for (int i = 0; i < dfaStateCount; i++) {
        for (int j = 0; j < inputCount; j++) {
            char next[MAX * 3] = "";
            char current[MAX * 3];
            strcpy(current, dfaStates[i]);
            getNextState(current, inputs[j], next);
            if (strlen(next) == 0) continue;

            int idx = isDFAStatePresent(next);
            if (idx == -1) {
                strcpy(dfaStates[dfaStateCount], next);
                dfaTrans[i][j] = dfaStateCount;
                dfaStateCount++;
            } else {
                dfaTrans[i][j] = idx;
            }
        }
    }
}

int main() {
    FILE *fp = fopen("nfa.txt", "r");
    if (!fp) {
        printf("Error: could not open file.\n");
        return 1;
    }

    printf("Enter number of NFA states: ");
    scanf("%d", &stateCount);
    printf("Enter state names (e.g., q0 q1 q2):\n");
    for (int i = 0; i < stateCount; i++) {
        scanf("%s", states[i]);
    }

    printf("Enter number of input symbols: ");
    scanf("%d", &inputCount);
    printf("Enter input symbols (e.g., a b):\n");
    for (int i = 0; i < inputCount; i++) {
        scanf("%s", inputs[i]);
    }

    char from[3], input[3], to[3];
    while (fscanf(fp, "%s %s %s", from, input, to) != EOF) {
        addTransition(from, input, to);
    }
    fclose(fp);

    convertNFAToDFA();

    printf("\nDFA Transition Table:\n");
    printf("State\t");
    for (int i = 0; i < inputCount; i++) printf("%s\t", inputs[i]);
    printf("\n");

    for (int i = 0; i < dfaStateCount; i++) {
        printf("{%s}\t", dfaStates[i]);
        for (int j = 0; j < inputCount; j++) {
            if (dfaTrans[i][j] == 0 && strcmp(dfaStates[0], "q0") != 0) {
                printf("-\t");
            } else {
                printf("{%s}\t", dfaStates[dfaTrans[i][j]]);
            }
        }
        printf("\n");
    }

    return 0;
}