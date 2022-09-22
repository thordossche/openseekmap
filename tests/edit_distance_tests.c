#include "edit_distance_tests.h"
#include "../src/edit_distance.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void run_edit_distance_tests() {
    // nothing
    assert(edit_distance("aaa", "aaa") == 0);
    assert(edit_distance("aAa", "aaa") == 0);
    assert(edit_distance("àAa", "aaa") == 0);
    assert(edit_distance("ÀAa", "aaa") == 0);
    assert(edit_distance("vrïjhëïdśtráát bruhè", "vrijheidstraat bruhe") == 0);

    // replace
    assert(edit_distance("aba", "aaa") == 1);
    assert(edit_distance("abb", "aaa") == 2);

    // insert
    assert(edit_distance("aa", "aaa") == 1);
    assert(edit_distance("a", "aaa") == 2);

    // delete
    assert(edit_distance("aaa", "aa") == 1);
    assert(edit_distance("aaa", "a") == 2);

    // swap
    assert(edit_distance("abc", "bac") == 1);
    assert(edit_distance("abc", "acb") == 1);

    // replace and insert
    assert(edit_distance("baa", "ra") == 2);
    
    // replace and delete
    assert(edit_distance("baa", "raaa") == 2);
    
    // insert and delete == same as replacing 2 times
    assert(edit_distance("aaax", "raaa") == 2);
    
    // swap and replace
    assert(edit_distance("abc", "bax") == 2);

    // swap and insert
    assert(edit_distance("abc", "ba") == 2);

    // swap and delete
    assert(edit_distance("abc", "ba") == 2);

    // swap and replace and insert
    assert(edit_distance("abc", "baxm") == 3);

    // swap and replace and delete
    assert(edit_distance("abcd", "bax") == 3);
    
    // extra
    assert(edit_distance("bb", "aaa") == 3);
    assert(edit_distance("bbbb", "aaa") == 4);
    assert(edit_distance("a", "aaa") == 2);
    assert(edit_distance("", "aaa") == 3);
    assert(edit_distance("bcc", "aaa") == 3);
    assert(edit_distance("acc", "aaacc") == 2);
    assert(edit_distance("cba", "abc") == 2);
    assert(edit_distance("fles", "flessen") == 3);
    assert(edit_distance("fiets", "fiets") == 0);
    assert(edit_distance("bbb", "b") == 2);
    assert(edit_distance("aaba", "aa") == 2);
    assert(edit_distance("a", "mbc") == 3);
    assert(edit_distance("mbc", "a") == 3);
    assert(edit_distance("abc", "") == 3);
}
