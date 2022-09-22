#!/usr/bin/env sh

echo "Starting to run tests"

echo "Compiling..."
gcc tests/runner.c tests/edit_distance_tests.c tests/shift_and_tests.c tests/entry_tests.c src/edit_distance.c src/shift_and.c src/entry.c src/linkedlist.c assignment/accents.c assignment/utf8.c -g -o tests/tests.o -Wall -Werror -std=c18

echo "Running tests"
#./tests/tests.o
valgrind --leak-check=full --show-leak-kinds=all -s ./tests/tests.o

echo "Removing test files"
rm ./tests/tests.o

exit 0
