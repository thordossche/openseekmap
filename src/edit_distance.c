#include "edit_distance.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "../assignment/utf8.h"
#include "../assignment/accents.h"
#define min(a,b) (a<b?a:b)

void print_matrix(uint8_t** d, int n, int n2) {
    for (int i = 0; i < n + 1; i++) {
        printf("[");
        for (int j = 0; j < n2 + 1; j++) {
            printf(" %i", d[i][j]);
        }
        printf(" ]\n");
    }
}

uint8_t** init(int n, int n2) {
    uint8_t** d = (uint8_t**)malloc((n+1)*sizeof(uint8_t*));
    for (int i = 0; i < n+1; i++) {
        d[i] = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));    
        for(int j = 0; j < n2+1; j++) {
            d[i][j] = 0;
        }
    }
    
    for(int i = 0; i < n + 1; i++) d[i][0] = i;

    for(int j = 0; j < n2 + 1; j++) d[0][j] = j;
    
    return d;
}

uint8_t min_3(uint8_t a, uint8_t b, uint8_t c) {
    return min(a, min(b, c));
}

int edit_distance_with_matrix(char* t, char* t2) {
    int n = strlen(t);
    int n2 = strlen(t2);

    uint8_t** d = init(n, n2);

    for (int i = 1; i < n + 1; i++) {
        for(int j = 1; j < n2 + 1; j++) {
            int cost;
            if (t[i-1] == t2[j-1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            d[i][j] = min_3(d[i-1][j-1] + cost, d[i-1][j] + 1, d[i][j-1] + 1);

            if (i > 1 && j > 1 && t[i-1] == t2[j-2] && t[i-2] == t2[j-1]) {
                d[i][j] = min(d[i][j], d[i-2][j-2] + 1);
            }
        }
    }

    int edit_distance = d[n][n2]; 

    for (int i = 0; i < n+1; i++) {
        free(d[i]);
    }
    free(d);

    return edit_distance;
}

int edit_distance_char(char* t, char* t2) {
    //return edit_distance_with_matrix(t, t2);
    int n = strlen(t);
    int n2 = strlen(t2);

    uint8_t* prev = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));
    uint8_t* next = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));
    uint8_t* swap = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));
    for(int i = 0; i < n2 + 1; i++) {
        prev[i] = i;
        next[i] = 0;
        swap[i] = 0;
    }

    uint8_t* tmp; 
    for (int i = 1; i < n + 1; i++) {
        next[0] = i;
        for(int j = 1; j < n2 + 1; j++) {
            int cost;
            if (tolower(t[i-1]) == tolower(t2[j-1])) {
                cost = 0;
            } else {
                cost = 1;
            }
            next[j] = min_3(prev[j-1] + cost, next[j-1] + 1, prev[j] + 1);

            if (i > 1 && j > 1 && tolower(t[i-1]) == tolower(t2[j-2]) && tolower(t[i-2]) == tolower(t2[j-1])) {
                next[j] = min(next[j], swap[j-2] + 1);
            }
        }

        tmp = swap;
        swap = prev;
        prev = next;
        next = tmp;
    }

    int edit_distance = prev[n2]; 
    
    free(prev);
    free(next);
    free(swap);
    return edit_distance;
}

int edit_distance(char* t, char* t2) {
    //return edit_distance_with_matrix(t, t2);
    int n2 = u8_strlen(t2);

    uint8_t* prev = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));
    uint8_t* next = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));
    uint8_t* swap = (uint8_t*)malloc((n2+1)*sizeof(uint8_t));
    for(int i = 0; i < n2 + 1; i++) {
        prev[i] = i;
        next[i] = 0;
        swap[i] = 0;
    }

    int t_counter = 0;
    int t2_counter = 0;

    uint32_t t_1 = u8_nextchar(t,&t_counter);
    uint32_t t_2 = t_1;
    uint32_t t2_1 = u8_nextchar(t2,&t2_counter);
    uint32_t t2_2 = t2_1;

    int i = 1;
    int j = 1;
    t_counter = 0;
    t2_counter = 0;

    uint8_t* tmp; 
    while((t_1 = u8_nextchar(t,&t_counter))){
        next[0] = i;
        j = 1;
        t2_counter = 0;
        while((t2_1 = u8_nextchar(t2,&t2_counter))){
            int cost;
            if (tolower(remove_accent(t_1)) == tolower(remove_accent(t2_1))) {
                cost = 0;
            } else {
                cost = 1;
            }
            next[j] = min_3(prev[j-1] + cost, next[j-1] + 1, prev[j] + 1);

            if (i > 1 && j > 1 && tolower(remove_accent(t_1)) == tolower(remove_accent(t2_2)) && tolower(remove_accent(t_2)) == tolower(remove_accent(t2_1))) {
                next[j] = min(next[j], swap[j-2] + 1);
            }
           
            t2_2 = t2_1; 
            j++;
        }
        t_2 = t_1; 
        i++;

        tmp = swap;
        swap = prev;
        prev = next;
        next = tmp;

    }

    int edit_distance = prev[n2]; 
    
    free(prev);
    free(next);
    free(swap);
    return edit_distance;
}
