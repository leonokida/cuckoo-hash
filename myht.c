// Leon Okida e Pedro Willian

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define M 11
#define REMOVED -100
#define NULL_TABLE -101
#define MAX_LEN 10

struct node {
    int key;
    int table; // 1 or 2
    int index;
};
typedef struct node node_t;

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
    // if (search_ht(k, *h1t, *h2t) != NULL_TABLE)
    //     return;

    int i1 = h1(k);

    // First case: the element does not exist in H1
    if ((*h1t)[i1] == NULL_TABLE || (*h1t)[i1] == REMOVED) {
        (*h1t)[i1] = k;
        return;
    }

    // Insert old element in H2 in case of a colision in H1

    int old_key = (*h1t)[i1];
    int i2 = h2(old_key);
    (*h2t)[i2] = old_key;
    (*h1t)[i1] = k;
}

// Remove element of the H1 or H2 table;
void remove_ht(int k, int **h1t, int **h2t) {
    // Cannot remove a non-existent element
    if (search_ht(k, *h1t, *h2t) == NULL_TABLE)
        return;

    // Search if the element is in H2
    int i2 = h2(k);

    if ((*h2t)[i2] != NULL_TABLE) {
        (*h2t)[i2] = NULL_TABLE;
        return;
    }

    // If it is in H1, it should be marked as REMOVED
    int i1 = h1(k);
    (*h1t)[i1] = REMOVED;
}

int compare_nodes(const void *a, const void *b) {
    node_t *nodeA = (node_t *)a;
    node_t *nodeB = (node_t *)b;

    if (nodeA->key < nodeB->key)
        return -1;
    else if (nodeA->key > nodeB->key)
        return 1;
    
    if (nodeA->table < nodeB->table)
        return -1;
    else if (nodeA->table > nodeB->table)
        return 1;
    
    if (nodeA->index < nodeB->index)
        return -1;
    else if (nodeA->index > nodeB->index)
        return 1;

    return 0;
}

int main() {
    // Initializes H1 and H2 hash tables
    int *h1t = create_ht(M);
    int *h2t = create_ht(M);

    // Reads instructions from stdin
    char line[MAX_LEN];
    int result;

    node_t* hash_nodes = malloc(M*2 * sizeof(node_t));
    if (hash_nodes == NULL){
        printf("Erro ao alocar memória\n");
        return 1;
    }

    for (int i = 0; i < M*2; i++) {
        hash_nodes[i].key = -1;   
        hash_nodes[i].table = 0;  
        hash_nodes[i].index = i; 
    }

    while (fgets(line, sizeof(line), stdin) != NULL) {
        // verify if is 'i' (insert) or 'r' (remove)
        if (line[0] == 'i') {
            sscanf(line, "i %d", &result);
            insert_ht(result, &h1t, &h2t);
        } else if (line[0] == 'r') {
            sscanf(line, "r %d", &result);
            remove_ht(result, &h1t, &h2t);
        } else {
            printf("Comando inválido\n");
        }
        
    }

    int count = 0;
    for(int i = 0; i < M; i++){
        if(h1t[i] != NULL_TABLE && h1t[i] != REMOVED){

            hash_nodes[count].index = i;
            hash_nodes[count].key = h1t[i];
            hash_nodes[count].table = 1;
            count++;
        }
        
        if(h2t[i] != NULL_TABLE && h2t[i] != REMOVED){
            hash_nodes[count].index = i;
            hash_nodes[count].key = h2t[i];
            hash_nodes[count].table = 2;
            count++;
        }

    }

    qsort(hash_nodes, M*2, sizeof(node_t), compare_nodes);
    

    for(int i = 0; i < M * 2; i++){
        if(hash_nodes[i].key != -1){
            printf("%d,T%d,%d\n", hash_nodes[i].key, hash_nodes[i].table, hash_nodes[i].index);
        }
    }

    // Frees allocated memory and ends program
    free(hash_nodes);
    free(h1t);
    free(h2t);
    return 0;
}