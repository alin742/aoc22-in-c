#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LINE_CAP 10*1024
#define WORD_CAP 10*124

bool isUpperCase(char c) {
  return c >= 'A' && c <= 'Z';
}

bool isLowerCase(char c) {
  return c >= 'a' && c <= 'z';
}

bool isAlphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isNumber(char c) {
    return c >= '0' && c <='9';
}

char *readLine(FILE *file_ptr) {
    char *line = (char *)malloc(sizeof(char)*LINE_CAP);
    memset(line, '\0', sizeof(char)*LINE_CAP);
    int i = 0;
    char c = '\0';
    while (!feof(file_ptr) && c != '\n') {
        c = fgetc(file_ptr);
        if (i > 0 && c == '\n') break;
        line[i++] = c;
    }
    return &line[0];
}

char *readWord(FILE *file_ptr) {
    char *word = (char *)malloc(sizeof(char) * WORD_CAP);
    memset(word, '\0', sizeof(char)*WORD_CAP);
    char c = '\0'; 
    int i = 0;
    while (!feof(file_ptr) && (c != ' ' || c != '\n')) {
        c = fgetc(file_ptr);
        if (c == ' ' || c == '\n') break;
        word[i++] = c;
    }
    return word;
}

#endif
