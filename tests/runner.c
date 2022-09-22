#include<stdio.h>
#include "edit_distance_tests.h"
#include "shift_and_tests.h"
#include "entry_tests.h"

int main(int argc, char* argv[]) {
    run_edit_distance_tests();
    run_shift_and_tests();
    run_entry_tests();
}
