#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "strings.h"

#if 1
    #define INPUT_FILE "input.txt"
#else
    #define INPUT_FILE "sample.txt"
#endif

#define CHILDREN_CAP 256
// #define POTOFFBUFF_CAP 1024

typedef struct {
    void *parent;
    char *name;
    bool dir;
    unsigned long size;
    void *children[CHILDREN_CAP];
    unsigned int children_count;
} tree;

tree *potential_offenders[CHILDREN_CAP];
int potential_offenders_sz = 0;

void printTree(tree *t) {
    printf("- %s ", t->name);
    if (!t->dir) printf("(file, size=%lu)", t->size);
    else printf("(dir, rec_size=%lu)", t->size);
    printf("\n");
    for (int i=0; i<t->children_count; i++) {
        tree *child = t->children[i];
        while (child->parent != NULL) {
            printf("    ");
            child = child->parent;
        }
        printTree(t->children[i]);
    }
}

tree *createTreeNode(char *dir) {
    tree *n = malloc(sizeof(tree));
    n->children_count = 0;
    n->name = dir;
    n->dir = true;
    n->parent = NULL;
    n->size = 0;
    return n;
}

bool addChild(tree *t, char *dir, unsigned long size) {
    tree *child = createTreeNode(dir);
    if (size > 0) child->dir = false;
    child->size = size;
    child->parent = t;
    if (t->children_count+1 < CHILDREN_CAP) {
        t->children[t->children_count] = child;
        t->children_count++;
        return true;
    }
    return false;
}

int updateSizes(tree *t) {
    if (!t->dir) return t->size;
    unsigned long size = 0;
    for (int i=0; i<t->children_count; i++) {
        size += updateSizes((tree *)t->children[i]);
    }
    t->size = size;
    return size;
}

tree *changeDir(tree *t, char *dir) {
    updateSizes(t);
    if (strcmp(dir, "..") == 0 && t->parent != NULL) {
        return t->parent;
    }
    for (int i=0; i < t->children_count; i++) {
        tree *child = (tree *)t->children[i];
        if (strcmp(child->name, dir) == 0) {
            return child;
        }
    }
    return t;
}

int findFoldersWithAtMost(tree *t, int threshold) {
    if (t->dir) {
        // printf("looking in %s\n", t->name);
        int size = 0;
        for (int i=0; i<t->children_count; i++)
            size += findFoldersWithAtMost(((tree*)t->children[i]), threshold);
        if (t->size < threshold)
            size += t->size;
        return size;
    }
    return 0;
}

void findFoldersBiggerThan(tree *t, unsigned long threshold)  {
    if (t->dir) {
        for (int i=0; i<t->children_count; i++) {
            findFoldersBiggerThan((tree*)t->children[i], threshold);
        }
        if (t->size >= threshold)
            potential_offenders[potential_offenders_sz++] = t;
    }
}

tree *generateFileTree(FILE *f) {
    int idx;
    char *line;
    char *word = malloc(CHILDREN_CAP*sizeof(char));
    bool listing = false;
    tree *t = createTreeNode("/");
    readLine(f);
    while(!feof(f)) {
        line = readLine(f);
        idx = contains(line, "$ ls", 4);
        if (idx > 0) {
            listing = true;
            continue;
        }
        idx = contains(line, "$ cd", 4);
        if (idx > 0) {
            listing = false;
            t = changeDir(t, line+idx+1);
            continue;
        }
        idx = contains(line, "dir", 3);
        if (idx > 0) {
            addChild(t, line+idx+1, 0);
            continue;
        }
        idx = contains(line, " ", 1);
        if (idx > 0) {
            strncpy(word, line, idx);
        }
        unsigned long size = (unsigned long)atoll(word);
        addChild(t, line+idx, size);
    }
    free(word);
    while (t->parent != NULL) {
        t = changeDir(t, "..");
    }
    updateSizes(t);
    return t;
}

void part1() {
    FILE *f = fopen(INPUT_FILE, "r");
    tree *t = generateFileTree(f);
    // printTree(t);
    int total = findFoldersWithAtMost(t, 100000);
    printf("part1: %d\n", total);
    fclose(f);
    free(t);
}

void part2() {
    FILE *f = fopen(INPUT_FILE, "r");
    tree *t = generateFileTree(f);
    unsigned long total_disk_space = 70*1000*1000;
    unsigned long required_disk_space = 30*1000*1000;
    unsigned long used_disk_space = t->size;
    unsigned long to_free = used_disk_space - (total_disk_space - required_disk_space);
    findFoldersBiggerThan(t, to_free);
    tree *smallest_offender = t;
    for (int i=0; i<potential_offenders_sz; i++) {
        if (potential_offenders[i]->size < smallest_offender->size && potential_offenders[i]->size > to_free)
            smallest_offender = potential_offenders[i];
    }
    printf("part2: %lu\n", smallest_offender->size);
    fclose(f);
    free(t);
}

int main(int argc, char **argv) {
    part1();
    part2();
    return 0;
}
