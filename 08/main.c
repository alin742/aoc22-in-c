#include <stdio.h>
#include "../common.h"

#if 1
    #define INPUT_FILE "input.txt"
    #define GRID_SZ 99
#else
    #define INPUT_FILE "sample.txt"
    #define GRID_SZ 5
#endif

int grid[GRID_SZ*GRID_SZ];
char *texture = " .:-=+*#%@";

int ctou8(char c) {
    return c - '0';
}

int get(int r, int c) {
    return grid[c+(r*GRID_SZ)];
}

void generateGrid(FILE *f) {
    int j = 0;
    char *line;
    while(!feof(f)) {
        line = readLine(f);
        for (int i=0; line[i] != '\n' && line[i] != '\0'; i++)
            grid[i+j] = ctou8(line[i]);
        j += GRID_SZ;
    }
}

void printGrid() {
    for (int i=0; i<GRID_SZ; i++) {
        for (int j=0; j<GRID_SZ; j++) {
            printf("%c", texture[get(i,j)]);
        }
        printf("\n");
    }
}

void printPos(int r, int c) {
    for (int i=0; i<GRID_SZ; i++) {
        for (int j=0; j<GRID_SZ; j++) {
            if (i == r && j == c) {
                char ESC = 27;
                printf("%c[0;31m",ESC);  /*- turn on red */
                printf("%d", get(i,j));
                printf("%c[0m",ESC); /* turn off red */
            }
            else printf("%d", get(i,j));
        }
        printf("\n");
    }
}
bool isInBounds(int r, int c) {
    return ( 0 <= r && r < GRID_SZ ) && ( 0 <= c && c < GRID_SZ );
}

bool isLeftLower(int r, int c, int v) {
    if (!isInBounds(r, c)) return true;
    if (get(r, c) >= v) return false;
    return isLeftLower(r, c-1, v);
}

bool isRightLower(int r, int c, int v) {
    if (!isInBounds(r, c)) return true;
    if (get(r, c) >= v) return false;
    return isRightLower(r, c+1, v);
}

bool isTopLower(int r, int c, int v) {
    if (!isInBounds(r, c)) return true;
    if (get(r, c) >= v) return false;
    return isTopLower(r-1, c, v);
}

bool isBottomLower(int r, int c, int v) {
    if (!isInBounds(r, c)) return true;
    if (get(r, c) >= v) return false;
    return isBottomLower(r+1, c, v);
}

int countUp(int r, int c, int v) {
    if (!isInBounds(r, c)) return 0;
    if (get(r, c) >= v) return 1;
    return countUp(r-1, c, v)+1;
}

int countDown(int r, int c, int v) {
    if (!isInBounds(r, c)) return 0;
    if (get(r, c) >= v) return 1;
    return (countDown(r+1, c, v)+1);
}

int countLeft(int r, int c, int v) {
    if (!isInBounds(r, c)) return 0;
    if (get(r, c) >= v) return 1;
    return countLeft(r, c-1, v)+1;
}

int countRight(int r, int c, int v) {
    if (!isInBounds(r, c)) return 0;
    if (get(r, c) >= v) return 1;
    return countRight(r, c+1, v)+1;
}

bool isVisible(int r, int c) {
    // DFS entry point
    bool visible = isLeftLower(r,c-1, get(r,c)) ||
        isRightLower(r,c+1, get(r,c)) ||
        isTopLower(r-1,c, get(r,c)) ||
        isBottomLower(r+1,c, get(r,c));
    return visible;
}

int countVisibleRegion(int r, int c) {
    int value = get(r,c);
    return (countUp(r-1, c,value)  * countLeft(r, c-1, value) * countRight(r, c+1, value) * countDown(r+1, c, value));
}

int countVisible() {
    int visible = 0;
    for (int i=0; i<GRID_SZ; i++) {
        for (int j=0; j<GRID_SZ; j++)
            if (isVisible(i, j)) visible++;
    }
    return visible;
}

void part1(){
    FILE *f = fopen(INPUT_FILE, "r");
    generateGrid(f);
    printf("part1: %d trees are visible\n", countVisible());
}

void part2(){
    FILE *f = fopen(INPUT_FILE, "r");
    generateGrid(f);
    int tc = 0;
    int tr = 0;
    int max_score = -1;
    for (int i = 0; i<GRID_SZ; i++) {
        for (int j = 0; j<GRID_SZ; j++) {
            int score = countVisibleRegion(i, j);
            if (score > max_score) {
                max_score = score;
                tc = j;
                tr = i;
            }
        }
    }
    printf("part2: %d is the highest scenic count\n", max_score);
}

int main(int argc, char **argv) {
    // part1();
    part2();
    return 0;
}
