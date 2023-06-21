#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1
  #define INPUT_FILE "input.txt"
#else
  #define INPUT_FILE "sample.txt"
#endif
#define NUMBER_SZ 256
#define NUMBER_CAP 1024

char *number;
int elves[NUMBER_CAP][NUMBER_CAP];

int readElvesCalories(FILE *file_ptr) {
  number = malloc(sizeof(char)*NUMBER_SZ);
  memset(number, 0, sizeof(char)*NUMBER_SZ);
  memset(elves, 0, sizeof(int)*NUMBER_CAP*NUMBER_CAP);
  int n_char = 0, n_calories = 0, n_elves = 0;
  
  while(!feof(file_ptr)) {
    char c = fgetc(file_ptr);
    if (c == '\n') {
      n_char = 0;
      elves[n_elves][n_calories] = atoi(number);
      char next_c = fgetc(file_ptr);
      n_calories++;
      memset(number, 0, sizeof(char)*NUMBER_SZ);
      if (next_c == '\n') {
        elves[n_elves][n_calories] = -1;
        n_calories = 0;
        n_elves++;
      } else {
        number[n_char++] = next_c;
      }
    } else {
      number[n_char++] = c;
    }
  }
  
  return n_elves;
}

void sumElvesCalories(int n_elves) {
  for (int i=0; i<n_elves; i++) {
    int j = 0;
    int sum = 0;
    while (elves[i][j] != -1) {
      sum += elves[i][j++];
    }
    elves[i][0] = sum;
    // printf("elf %d: %d\n", i, *elves[i]);
  }
}

int maxCalories(int n_elves) {
  int max = INT_MIN;
  for (int i = 0; i < n_elves; i++) 
    if (*elves[i] > max) max = *elves[i];
  return max;
}

int sumTop3Calories(int n_elves) {
  int temp;
  for (int i = 0; i < n_elves; i++) {
    for (int j = 0; j < n_elves - i; j++) {
      if (*elves[j] < *elves[j+1]) {
        temp = *elves[j+1];
        *elves[j+1] = *elves[j];
        *elves[j] = temp;
      }
    }
  }
  return *elves[0] + *elves[1] + *elves[2];
}

void part1() {
  FILE *file_ptr = fopen(INPUT_FILE, "r");
  int n_elves = readElvesCalories(file_ptr);
  sumElvesCalories(n_elves);
  printf("Max Calories: %d\n", maxCalories(n_elves));
}

void part2() {
  FILE *file_ptr = fopen(INPUT_FILE, "r");
  int n_elves = readElvesCalories(file_ptr);
  sumElvesCalories(n_elves);
  int sum = sumTop3Calories(n_elves);
  printf("Sum of top 3 Calories: %d\n", sum);
}

int main(int argc, char **argv) {
  part1();
  part2();
  return 0;
}
