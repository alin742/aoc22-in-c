#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1
#define INPUT_FILE "input.txt"
#else
#define INPUT_FILE "sample.txt"
#endif
#define LINE_CAP 256
#define NUM_LINES 512

struct Rucksack {
  char items[LINE_CAP];
  int size;
};

struct Rucksack rucksacks[NUM_LINES];
struct Rucksack split_rucksacks[NUM_LINES][2];

bool isLowerCase(char c) { return c >= 'a' && c <= 'z'; }

bool isUpperCase(char c) { return c >= 'A' && c <= 'Z'; }

bool isAlphabet(char c) { return isLowerCase(c) || isUpperCase(c); }

int fillRucksack(struct Rucksack *r, char c) {
  r->items[r->size] = c;
  r->size += 1;
  return r->size;
}

void initializeRucksack(struct Rucksack *r) {
  r->size = 0;
  memset(&(r->items), 0, LINE_CAP);
}

void showRucksack(struct Rucksack *r) {
  printf("rucksack content: [%s], number of content: %d\n", r->items, r->size);
}

void splitRucksack(int index) {
  int n_chars = rucksacks[index].size / 2;
  split_rucksacks[index][0].size = n_chars;
  split_rucksacks[index][1].size = n_chars;
  for (int i = 0; i < rucksacks[index].size; i++) {
    if (i < n_chars) {
      split_rucksacks[index][0].items[i] = rucksacks[index].items[i];
    } else {
      split_rucksacks[index][1].items[i - n_chars] = rucksacks[index].items[i];
    }
  }
}

char getCommonItem(struct Rucksack split_rucksack[2]) {
  for (int i = 0; i < split_rucksack[0].size; i++) {
    for (int j = 0; j < split_rucksack[1].size; j++) {
      // printf("%c ~ %c\n", split_rucksack[0].items[i],
      // split_rucksack[1].items[j]);
      if (split_rucksack[0].items[i] == split_rucksack[1].items[j])
        return split_rucksack[0].items[i];
    }
  }
  printf("Unreachable\n");
  return -1;
}

int calculateItemScore(char c) {
  if (isLowerCase(c)) {
    return (c - 'a') + 1;
  } else if (isUpperCase(c)) {
    return (c - 'A') + 27;
  }
  return 0;
}

int readData(FILE *file_ptr) {
  int n_rucksacks = 0;
  while (!feof(file_ptr)) {
    char c = fgetc(file_ptr);
    if (c == '\n') {
      n_rucksacks++;
      initializeRucksack(&rucksacks[n_rucksacks]);
    } else {
      if (isAlphabet(c))
        fillRucksack(&rucksacks[n_rucksacks], c);
    }
  }
  return n_rucksacks;
}

bool itemInRucksack(struct Rucksack *r, char c) {
  for (int i = 0; i < r->size; i++) {
    if (r->items[i] == c)
      return true;
  }
  return false;
}

char getGroupBadge(struct Rucksack r1, struct Rucksack r2, struct Rucksack r3) {
  for (int i = 0; i < r1.size; i++) {
    if (itemInRucksack(&r2, r1.items[i]) && itemInRucksack(&r3, r1.items[i]))
      return r1.items[i];
  }
  printf("Unreachable\n");
  return '\0';
}

void part1() {
  FILE *f = fopen(INPUT_FILE, "r");
  int n_rucksacks = readData(f);
  int score = 0;
  for (int i = 0; i <= n_rucksacks; i++) {
    splitRucksack(i);
    char common_item = getCommonItem(split_rucksacks[i]);
    score += calculateItemScore(common_item);
  }
  printf("part1 score: %d\n", score);
}

void part2() {
  FILE *f = fopen(INPUT_FILE, "r");
  int n_rucksacks = readData(f) + 1;
  int groups = n_rucksacks / 3;
  int score = 0;
  for (int i = 0; i < groups; i++) {
    char badge = getGroupBadge(rucksacks[0 + 3 * i], rucksacks[1 + 3 * i],
                               rucksacks[2 + 3 * i]);
    score += calculateItemScore(badge);
  }
  printf("par2 score: %d\n", score);
}

int main(int argc, char **argv) {
  part1();
  part2();
  return 0;
}
