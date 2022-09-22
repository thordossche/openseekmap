#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "query.h"
#include "../assignment/utf8.h"

struct Query* create_query(char query_string[]) {
    // malloc query
    struct Query* query = (struct Query*)malloc(sizeof(struct Query));

    // calculate length
    int size = 0;

    int t_counter = 0;
    uint32_t t = 0;
    while((t = u8_nextchar(query_string ,&t_counter))){
        if (t == ' ') {
            size++;
        } 
    }
    query->size = size+1;

    // split query string
    char** query_array = (char**)malloc(query->size*sizeof(char*));

    // copy string so original does not get destroyed
    int n = strlen(query_string);
    char query_string_cpy[n+1]; 
    strcpy(query_string_cpy, query_string);

    // fill array
    char* token = strtok(query_string_cpy, " ");
    for (int i = 0; i < query->size; i++) {
        // malloc
        query_array[i] = (char*)malloc((strlen(token)+1)*sizeof(char));
        // copy part
        strcpy(query_array[i], token);
        // get next part
        token = strtok(NULL, " ");
    }
    query->query = query_array;

    return query;
}

void free_query(struct Query* query) {
    for (int i = 0; i < query->size; i++) {
        free(query->query[i]);
    }
    free(query->query);
    free(query);
}
