#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define M 11
#define REMOVED -100
#define NULL_TABLE -101
#define MAX_LEN 10

int *create_ht(int size) {
    int *ht = calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
        ht[i] = NULL_TABLE;
    }
    return ht;
}

// Hash function of the 1st hash table
// h1(k) = k mod m
int h1(int k) {
    return k % M;
}

// Hash function of the 2nd hash table
// h2(k) = ⌊m * (k * 0.9 − ⌊k * 0.9⌋)⌋
int h2(int k) {
    return (int)floor(M * (k * 0.9 - floor(k * 0.9)));
}

// Search in the H1 and H2 tables
int search_ht(int k, int *h1t, int *h2t) {
    int i1 = h1(k);

    // First case: the element does not exist in the table
    if (h1t[i1] == NULL_TABLE)
        return NULL_TABLE;

    // The element exists in either H1 or H2
    if (h1t[i1] == k)
        return i1;
    else
        return h2(k);
}

// Insert element in the H1 or H2 table
void insert_ht(int k, int **h1t, int **h2t) {
    // Ignore duplicates
    if (search_ht(k, *h1t, *h2t) != NULL_TABLE)
        return;

    int i1 = h1(k);

    // First case: the element does not exist in H1
    if ((*h1t[i1] == NULL_TABLE) || (*h1t[i1] == REMOVED)) {
        *h1t[i1] = k;
        return;
    }

    // Insert old element in H2 in case of a colision in H1
    int old_key = *h1t[i1];
    int i2 = h2(old_key);
    *h2t[i2] = old_key;
    *h1t[i1] = k;
}

// Remove element of the H1 or H2 table;
void remove_ht(int k, int **h1t, int **h2t) {
    // Cannot remove a non-existent element
    if (search_ht(k, *h1t, *h2t) == NULL_TABLE)
        return;

    // Search if the element is in H2
    int i2 = h2(k);
    if (*h2t[i2] != NULL_TABLE) {
        *h2t[i2] = NULL_TABLE;
        return;
    }

    // If it is in H1, it should be marked as REMOVED
    int i1 = h1(k);
    *h1t[i1] = REMOVED;
}

int main() {
    // Initializes H1 and H2 hash tables
    int *h1t = create_ht(M);
    int *h2t = create_ht(M);

    // Reads instructions from stdin
    char line[MAX_LEN], *result;
    while ((result = fgets(line, MAX_LEN, stdin)) != NULL)
        printf("The string is %s\n", result);

    // Frees allocated memory and ends program
    free(result);
    free(h1t);
    free(h2t);
    return 0;
}