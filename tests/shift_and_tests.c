#include "shift_and_tests.h"
#include "../src/shift_and.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void run_shift_and_tests() {

    assert(shift_and("vrïjhëïdśtráát bruhè", "vrijheidstraat bruhe", 0));
    // distance 0 
    assert(shift_and("aaa", "aaa", 0));
    assert(!shift_and("baa", "aaa", 0));
    assert(shift_and("àAa", "aaa", 0));
    assert(shift_and("ÀAa", "aaa", 0));

    //distance 1

    //should still work even with dist 1
    assert(shift_and("aaa", "aaa", 1));

    //insert
    assert(shift_and("aaa", "aa", 1));
    assert(!shift_and("aaa", "a", 1));

    //delete
    assert(shift_and("aaa", "baaa", 1));
    assert(!shift_and("aaa", "a", 1));

    //replace
    assert(shift_and("aaa", "baa", 1));
    assert(!shift_and("aaa", "bba", 1));

    //distance 2
    
    //insert & delete
    assert(shift_and("abcde", "acxde", 2));
    assert(!shift_and("abcde", "axxcxde", 2));
    
    //insert & replace
    assert(shift_and("abcde", "xxacfe", 2));
    assert(!shift_and("abcde", "xxafe", 2));

    //delete & replace
    assert(shift_and("abcdef", "abxcdrf", 2));
    assert(!shift_and("abcdef", "axbxcdrf", 2));
    
    //distance 3
    
    //insert & delete & replace
    assert(shift_and("abcdef", "bxcdrf", 3));
    assert(!shift_and("abcdef", "bxcdrxf", 3));
    void run_shift_and_tests_char();
    run_shift_and_tests_char(); 
}
void run_shift_and_tests_char() {
    // distance 0 
    assert(shift_and_char("aaa", "aaa", 0));
    assert(!shift_and_char("baa", "aaa", 0));

    //distance 1

    //should still work even with dist 1
    assert(shift_and_char("aaa", "aaa", 1));

    //insert
    assert(shift_and_char("aaa", "aa", 1));
    assert(!shift_and_char("aaa", "a", 1));

    //delete
    assert(shift_and_char("aaa", "baaa", 1));
    assert(!shift_and_char("aaa", "a", 1));

    //replace
    assert(shift_and_char("aaa", "baa", 1));
    assert(!shift_and_char("aaa", "bba", 1));

    //distance 2
    
    //insert & delete
    assert(shift_and_char("abcde", "acxde", 2));
    assert(!shift_and_char("abcde", "axxcxde", 2));
    
    //insert & replace
    assert(shift_and_char("abcde", "xxacfe", 2));
    assert(!shift_and_char("abcde", "xxafe", 2));

    //delete & replace
    assert(shift_and_char("abcdef", "abxcdrf", 2));
    assert(!shift_and_char("abcdef", "axbxcdrf", 2));
    
    //distance 3
    
    //insert & delete & replace
    assert(shift_and_char("abcdef", "bxcdrf", 3));
    assert(!shift_and_char("abcdef", "bxcdrxf", 3));
}
