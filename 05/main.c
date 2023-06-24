#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#if 1
    #define INPUT_FILE "input.txt"
    #define N_STACKS 9
#else
    #define INPUT_FILE "sample.txt"
    #define N_STACKS 3
#endif
#define COMMAND_CAP 1024
#define WORD_CAP 124
#define LINE_CAP 1024
#define N_STRIDE 4

typedef char Crate;

typedef struct {
    Crate crates[N_STACKS*N_STACKS];
    int   size;
} CargoStack;

CargoStack stacks[N_STACKS+1];

void initStacks() {
    for (int i=0; i<N_STACKS; i++) {
        stacks[i].size = 0;
    }
}

void revStack(int stack) {
    Crate rev[N_STACKS*N_STACKS];
    int N = stacks[stack].size;
    for (int i=0; i<N; i++) {
        rev[i] = stacks[stack].crates[N-i-1];
    }
    for (int i=0; i<N; i++) {
        stacks[stack].crates[i] = rev[i];
    }
}

void revStacks() {
    for (int i=0; i<N_STACKS; i++) {
        revStack(i);
    }
}

void pushCrate(int stack, Crate crate) {
    stacks[stack].crates[stacks[stack].size++] = crate;
}

Crate popCrate(int stack) {
    stacks[stack].size--;
    Crate crate = stacks[stack].crates[stacks[stack].size];
    return crate;
}

void moveCrate(int amount, int from, int to) {
    Crate c;
    for (int i=0; i<amount; i++) {
        c = popCrate(from-1);
        pushCrate(to-1, c);
    }
}

void showCrate(Crate c) {
    printf("[%c]", c);
}

void showStacks() {
    CargoStack *curr_stack;
    for (int i=0; i<N_STACKS; i++) {
        curr_stack = &stacks[i];
        printf("Stack %d [%d crates]: ", i+1, curr_stack->size);
        for (int j=0; j<curr_stack->size; j++) {
            showCrate(curr_stack->crates[j]);
        }
        printf("\n");
    }
}

void showTopOfStacks() {
    for (int i=0; i<N_STACKS; i++) {
        showCrate(stacks[i].crates[stacks[i].size-1]);
    }
    printf("\n");
}

bool isAlphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isNumber(char c) {
    return c >= '0' && c <='9';
}

char *readLine(FILE *file_ptr) {
    char *line = malloc(sizeof(char)*LINE_CAP);
    int i = 0;
    char c = '\0';
    while (!feof(file_ptr) && c != '\n') {
        c = fgetc(file_ptr);
        if (i > 0 && c == '\n') break;
        line[i++] = c;
    }
    line[i] = '\0';
    return &line[0];
}

char *readWord(FILE *file_ptr) {
    char *word = malloc(sizeof(char)*WORD_CAP);
    char c = '\0'; 
    int i = 0;
    while (!feof(file_ptr) && (c != ' ' || c != '\n')) {
        c = fgetc(file_ptr);
        if (c == ' ' || c == '\n') break;
        word[i++] = c;
    }
    word[i] = '\0';
    return word;
}

bool containsNumber(char *line) {
    int i = 0;
    char c = line[i++];
    while (c != '\0') {
        if (isNumber(c)) return true;
        c = line[i++];
    }
    return false;
}

int readCrates(FILE *file_ptr) {
    int n_crates = 0;
    char *line;
    char cargo;
    while (!containsNumber(line)) {
        line = readLine(file_ptr);
        if (containsNumber(line)) break;
        for (int i = 0; i < N_STACKS; i++) {
            for (int j = 0; j<N_STRIDE; j++) {
                cargo = line[j+i*N_STRIDE];
                if(j == 1 && cargo != ' ') {
                    pushCrate(i, cargo);
                    n_crates++;
                }
            }
        }
        free(line);
    }
    return n_crates;
}

void crateMover9000(FILE *file_ptr) {
    int n_commands = 0, amount, from, to;
    while (!feof(file_ptr)) {
        readWord(file_ptr);
        amount = atoi(readWord(file_ptr));
        readWord(file_ptr);
        from = atoi(readWord(file_ptr));
        readWord(file_ptr);
        to = atoi(readWord(file_ptr));
        moveCrate(amount, from, to);
    }
}

void crateMover9001(FILE *file_ptr) {
    int n_commands = 0, amount, from, to;
    while (!feof(file_ptr)) {
        readWord(file_ptr);
        amount = atoi(readWord(file_ptr));
        readWord(file_ptr);
        from = atoi(readWord(file_ptr));
        readWord(file_ptr);
        to = atoi(readWord(file_ptr));
        // move crates to crateMover
        moveCrate(amount, from, N_STACKS+1);
        // move crates from crateMover
        moveCrate(amount, N_STACKS+1, to);
    }
}

void part1() {
    initStacks();
    FILE *f = fopen(INPUT_FILE, "r");
    int n_crates = readCrates(f);
    revStacks();
    fgetc(f);
    fgetc(f);
    crateMover9000(f);
    printf("part1: ");
    showTopOfStacks();
    fclose(f);
}

void part2() {
    initStacks();
    FILE *f = fopen(INPUT_FILE, "r");
    int n_crates = readCrates(f);
    revStacks();
    fgetc(f);
    fgetc(f);
    crateMover9001(f);
    printf("part2: ");
    showTopOfStacks();
    fclose(f);
}

int main(int argc, char **argv) {
    part1();
    part2();
    return 0;
}
