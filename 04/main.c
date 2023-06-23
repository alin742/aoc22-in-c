#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if 1
    #define INPUT_FILE "input.txt"
    #define RANGE_CAP 100
#else
    #define INPUT_FILE "sample.txt"
    #define RANGE_CAP 10
#endif
#define NUM_ASSIGNMENTS 2048

typedef struct {
    int start;
    int end;
} Range;

Range assignments[NUM_ASSIGNMENTS][2];

bool include(Range *r1, Range *r2) {
    return r1->start <= r2->start && r1->end >= r2->end;
}

bool overlap(Range *r1, Range *r2) {
    return r1->start <= r2->end && r2->start <= r1->end;
}

void visualizeRange(Range *r) {
    for (int i=0; i<RANGE_CAP; i++) {
	if (i >= r->start && i <= r->end) printf("+");
	else printf(".");
    }
    printf("\n");
}

int readNumber(FILE *file_ptr) {
    int sz = log10(RANGE_CAP);
    char c[sz];
    bool read = true;
    memset(c, 0, sz);
    int i = 0;
    do {
	c[i] = fgetc(file_ptr);
	if (!(c[i] == '-' || c[i] == ',' || c[i] == '\n')) {
	    i++;
	} else if (i > 0) {
	    read = false;
	}
    } while(i < sz && read);
    return atoi(c);
}

int readData(FILE *file_ptr) {
    int n_assignments = 0;
    int val;
    char c[RANGE_CAP/10];
    int i = 0;
    while(!feof(file_ptr)) {
	Range r1;
	Range r2;

	val = readNumber(file_ptr);
	r1.start = val;
	val = readNumber(file_ptr);
	r1.end = val;
	assignments[n_assignments][0] = r1;

	val = readNumber(file_ptr);
	r2.start = val;
	val = readNumber(file_ptr);
	r2.end = val;
	assignments[n_assignments][1] = r2;

 	n_assignments++;
    }
    return n_assignments - 1;
}

void part1() {
    FILE *f = fopen(INPUT_FILE, "r");
    int n_assignments = readData(f);
    int fully_inclusive = 0;
    for (int i = 0; i<n_assignments; i++) {
	Range r1 = assignments[i][0];
	Range r2 = assignments[i][1];
	if (include(&r1, &r2) || include(&r2,&r1)) fully_inclusive++;
    }
    printf("part1 score: %d\n", fully_inclusive);
}

void part2() {
    FILE *f = fopen(INPUT_FILE, "r");
    int n_assignments = readData(f);
    int n_overlaps = 0;
    for (int i = 0; i<n_assignments; i++) {
	Range r1 = assignments[i][0];
	Range r2 = assignments[i][1];
	if (include(&r1, &r2) || include(&r2,&r1) || overlap(&r1,&r2) || overlap(&r2, &r1)) n_overlaps++;
    }
    printf("part2 score: %d\n", n_overlaps);
}

int main(int argc, char **argv) {
    part1();
    part2();
    return 0;
}
