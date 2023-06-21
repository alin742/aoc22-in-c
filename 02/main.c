#include <assert.h>
#include <stdio.h>

#if 1
  #define INPUT_FILE "input.txt"
#else
  #define INPUT_FILE "sample.txt"
#endif
#define FILE_CAP 3000
                //     1  2  3
                // P:  r  p  s  \\  O:
int score_table[9] = { 4, 8, 3,  // r
                       1, 5, 9,  // p
                       7, 2, 6}; // s

                   //     1  2  3
                   // P:  l  d  w  \\  O:
int strategy_table[9] = { 3, 4, 8,  // r
                          1, 5, 9,  // p
                          2, 6, 7}; // s

char player[FILE_CAP];
char opponent[FILE_CAP];
int calculateScore(int player, int opponent) {
  return score_table[player+3*opponent];
}

int readStrategy(FILE *file_ptr) {
  int n_games = 0;
  while(!feof(file_ptr)) {
    opponent[n_games] = fgetc(file_ptr) - 'A'; fgetc(file_ptr);
    player[n_games] = fgetc(file_ptr) - 'X'; fgetc(file_ptr);
    n_games++;
  }
  return n_games;
}

int secondStrategy(int player, int opponent) {
  return strategy_table[player+3*opponent];
}

void part1() {
  FILE *f = fopen(INPUT_FILE, "r");
  int n_games = readStrategy(f);
  int score = 0;
  for (int i=0; i<n_games; i++) {
    score += calculateScore(player[i], opponent[i]);
  }
  printf("Score: %d\n", score);
}

void part2() {
  FILE *f = fopen(INPUT_FILE, "r");
  int n_games = readStrategy(f);
  int score = 0;
  for (int i=0; i<n_games; i++) {
    score += secondStrategy(player[i], opponent[i]);
  }
  printf("Score: %d\n", score);
}

int main(int argc, char **argv) {
  part1();
  part2();
  return 0;
}
