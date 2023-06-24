// EZ
#include "../common.h"

#if 1
    #define INPUT_FILE "input.txt"
#else
    #define INPUT_FILE "sample.txt"
#endif

bool isUnique(char *c, int buff_sz) {
    for (int i=0; i<buff_sz; i++) {
        for (int j=i+1; j<buff_sz; j++) {
            if (c[i] == c[j]) return false;
        }
    }
    return true;
}

int findStartOfPacket(char *line, int buff_sz) {
    int i = buff_sz;
    char *search_buff = malloc(sizeof(char)*buff_sz);
    while(line[i] != '\0') {
        memset(search_buff, '\0', buff_sz);
        for (int j=0; j<buff_sz; j++) {
            search_buff[j] = line[i-buff_sz+j];
        }
        // printf("Searching on %s with index %d\n", search_buff, i);
        if (isUnique(search_buff, buff_sz)) return i;
        i++;
    }
    return i;
}

void part1() {
    FILE *f = fopen(INPUT_FILE, "r");
    int start_marker;
    while(!feof(f)) {
        start_marker = findStartOfPacket(readLine(f), 4);
        if (feof(f)) break;
        printf("part1: %d\n", start_marker);
    }
    fclose(f);
}

void part2() {
    FILE *f = fopen(INPUT_FILE, "r");
    int start_marker;
    while(!feof(f)) {
        start_marker = findStartOfPacket(readLine(f), 14);
        if (feof(f)) break;
        printf("part2: %d\n", start_marker);
    }
    fclose(f);
}

int main(int argc, char **argv) {
    part1();
    part2();
    return 0;
}
