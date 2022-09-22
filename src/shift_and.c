#include "shift_and.h"
#include "stdio.h"
#include "stdint.h"
#include <string.h>
#include <ctype.h>
#include "../assignment/utf8.h"
#include "../assignment/accents.h"

uint64_t shift_1(uint64_t x) {
    return (x << 1) | 1;
}

void print_binary(unsigned char c) {
    unsigned char i1 = (1 << (sizeof(c)*8-1));
    for(; i1; i1 >>= 1)
        printf("%d",(c&i1)!=0);
    printf("\n");
}

void get_characteristic_vectors(char* z, uint64_t* C) {
    for (int i = 0; i < 128; i++) {
        C[i] = 0;
    }
    for (int i = 0; i < strlen(z); i++) {
        C[(uint8_t)tolower(z[i])] |= (1 << i);
    }
}

int shift_and_k_0(char* z, char* t) {
    int m = strlen(z);
    int n = strlen(t);
    uint64_t C[128];

    get_characteristic_vectors(z, C);

    uint64_t R;
    if (z[0] == t[0]) {
        if (m == 1) return 1;
        R = 1;
    } else {
        R = 0;
    }

    for (int i = 1; i < n; i++) {
        R = shift_1(R) & C[(uint8_t)t[i]];
        if (R & (1 << (m-1))) return 1;
    }
    return 0;
}

int shift_and_char(char* z, char* t, int k) {
    int m = strlen(z);
    int n = strlen(t);
    uint64_t C[128];

    get_characteristic_vectors(z, C);

    uint64_t R[k+1];
    for (int i = 0; i <= k; i++) {
        R[i] = 1;
    }

    if (tolower(z[0]) == tolower(t[0])) {
        if (m == 1) return 1;
        R[0] = 1;
    } else {
        R[0] = 0;
    }

    uint64_t before;
    uint64_t prev;

    for (int i = 1; i < n; i++) {
        prev = R[0];
        R[0] = shift_1(R[0]) & C[(uint8_t)tolower(t[i])];
        for(int l = 1; l <= k; l++) {

            before = R[l];
            //     1 | Ins  |   Deletion    | Substitution,            Match
            R[l] = 1 | prev | (R[l-1] << 1) | (prev << 1) | ((R[l] << 1) & C[(uint8_t)tolower(t[i])]);
            prev = before;
        }
        if (R[k] & (1 << (m-1))) return 1;
    }
    return 0;
}

void get_characteristic_vectors_utf8(char* z, uint64_t* C) {
    for (int i = 0; i < 128; i++) {
        C[i] = 0;
    }

    int t_counter = 0;
    uint32_t t = 0;
    int i = 0;
    while((t = u8_nextchar(z,&t_counter))){
        // if ascii update vectors
        if (tolower(remove_accent(t)) < 128) {
            C[(uint8_t)tolower(remove_accent(t))] |= (1 << i);
        } 
        i++;
    }
}

uint64_t C(uint64_t* C_ascii, char* z, uint32_t x) {
    // if ascii
    if (tolower(remove_accent(x)) < 128) {
        return C_ascii[tolower(remove_accent(x))];
    }

    uint64_t c = 0;
    int t_counter = 0;
    uint32_t t = 0;
    int i = 0;

    while((t = u8_nextchar(z,&t_counter))){
        if (tolower(t) == x) {
           c |= (1 << i);
        } 
        i++;
    }
    return c;
}

int shift_and(char* z, char* t, int k) {
    int m = u8_strlen(z);
    uint64_t C_ascii[128];

    get_characteristic_vectors_utf8(z, C_ascii);

    uint64_t R[k+1];
    for (int i = 0; i <= k; i++) {
        R[i] = 1;
    }

    int z_counter = 0;
    int t_counter = 0;
    if (tolower(remove_accent(u8_nextchar(z,&z_counter))) == tolower(remove_accent(u8_nextchar(t,&t_counter)))) {
        if (m == 1) return 1;
        R[0] = 1;
    } else {
        R[0] = 0;
    }

    uint64_t before;
    uint64_t prev;

    t_counter = 0;
    uint32_t ti = u8_nextchar(t,&t_counter);

    while((ti = u8_nextchar(t,&t_counter))){
        prev = R[0];
        R[0] = shift_1(R[0]) & C(C_ascii, z, tolower(remove_accent(ti)));
        for(int l = 1; l <= k; l++) {

            before = R[l];
            //     1 | Ins  |   Deletion    | Substitution,            Match
            R[l] = 1 | prev | (R[l-1] << 1) | (prev << 1) | ((R[l] << 1) & C(C_ascii, z, tolower(remove_accent(ti))));
            prev = before;
        }
        if (R[k] & (1 << (m-1))) return 1;
    }
    return 0;
}
